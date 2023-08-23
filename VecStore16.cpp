// 增加多维度查询方法，允许通过组合多个属性进行查询。
#include <iostream> // 引入输入输出流库
#include <vector>   // 引入向量库
#include <string>   // 引入字符串库
#include <ctime>    // 引入时间库
#include <unordered_map> // 引入哈希表库
#include <cmath>    // 引入数学库
#include <limits>   // 引入极限库

class VectorData { // 定义向量数据类
public:
    std::vector<double> vector; // 存储向量的元素
    std::string label;          // 向量的标签
    std::string description;    // 向量的描述
    time_t timestamp;           // 向量的时间戳

    VectorData() : timestamp(std::time(0)) {} // 默认构造函数

    // 构造函数，可以接受向量、标签和描述
    VectorData(const std::vector<double>& vec, const std::string& lbl = "", const std::string& desc = "", time_t ts = std::time(0))
        : vector(vec), label(lbl), description(desc), timestamp(ts) {}

    // 输出向量的方法，方便调试
    void print() const {
        std::cout << "Label: " << label << "\nDescription: " << description << "\nTimestamp: " << std::ctime(&timestamp) << "Vector: ";
        for (double value : vector) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

class VectorStorage { // 定义向量存储类
public:
    
    // 添加向量到存储中的方法，同时包括元数据
    void addVector(const VectorData& data) {
        vectors.push_back(data);
        labelIndex[data.label] = vectors.size() - 1; // 更新标签索引
    }

    // 批量添加向量到存储中的方法
    void addVectors(const std::vector<VectorData>& datas) {
        for (const VectorData& data : datas) {
            addVector(data);
        }
    }

    // 通过索引查询向量的方法
    VectorData getVectorByIndex(size_t index) const {
        if (index < vectors.size()) return vectors[index];
        return VectorData(std::vector<double>());
    }

    // 通过索引获取一个向量的标签
    std::string getLabelByIndex(size_t index) const {
        if (index >= vectors.size()) {
            std::cerr << "Error: Index out of range!\n";
            return "";
        }
        return vectors[index].label;
    }

    // 通过索引获取一个向量的时间戳
    time_t getTimestampByIndex(size_t index) const {
        if (index >= vectors.size()) {
            std::cerr << "Error: Index out of range!\n";
            return 0;
        }
        return vectors[index].timestamp;
    }

    // 通过索引删除指定的向量的方法
    void deleteVectorByIndex(size_t index) {
        if (index < vectors.size()) vectors.erase(vectors.begin() + index);
    }

    // 通过索引更新指定的向量的方法
    void updateVectorByIndex(size_t index, const VectorData& data) {
        if (index < vectors.size()) vectors[index] = data;
    }

    // 查询存储中的向量数量的方法
    size_t size() const {
        return vectors.size();
    }

    // 清空所有存储的向量的方法
    void clear() {
        vectors.clear();
    }

    // 打印所有存储的向量的方法，方便调试
    void printAll() const {
        for (const VectorData& vecData : vectors) vecData.print();
    }

    // 通过标签查询向量的方法
    VectorData getVectorByLabel(const std::string& label) const {
        auto it = labelIndex.find(label);
        if (it != labelIndex.end()) return vectors[it->second];
        return VectorData(std::vector<double>());
    }

    // 基于相似度的搜索方法，找到与给定向量最相似的向量
    VectorData findMostSimilarVector(const std::vector<double>& targetVector) const {
        double minDistance = std::numeric_limits<double>::max();
        VectorData mostSimilar;
        for (const VectorData& vecData : vectors) {
            double distance = computeEuclideanDistance(targetVector, vecData.vector);
            if (distance < minDistance) {
                minDistance = distance;
                mostSimilar = vecData;
            }
        }
        return mostSimilar;
    }

    // 按照时间戳范围查询向量的方法
    std::vector<VectorData> getVectorsByTimestampRange(time_t start, time_t end) const {
        std::vector<VectorData> result;
        for (const VectorData& vecData : vectors) {
            if (vecData.timestamp >= start && vecData.timestamp <= end) {
                result.push_back(vecData);
            }
        }
        return result;
    }

    // 多维度查询方法，允许通过组合多个属性进行查询
    std::vector<VectorData> queryVectors(const std::string& label = "", const std::string& description = "", time_t start = 0, time_t end = std::numeric_limits<time_t>::max()) const {
        std::vector<VectorData> result;
        for (const VectorData& vecData : vectors) {
            if ((label.empty() || vecData.label == label) &&
                (description.empty() || vecData.description == description) &&
                (vecData.timestamp >= start && vecData.timestamp <= end)) {
                result.push_back(vecData);
            }
        }
        return result;
    }

private:
    std::vector<VectorData> vectors; // 存储所有向量的容器
    std::unordered_map<std::string, size_t> labelIndex; // 通过标签快速查找向量的索引

    // 计算两个向量之间的欧几里得距离
    double computeEuclideanDistance(const std::vector<double>& vec1, const std::vector<double>& vec2) const {
        double sum = 0.0;
        for (size_t i = 0; i < vec1.size(); ++i) {
            sum += std::pow(vec1[i] - vec2[i], 2);
        }
        return std::sqrt(sum);
    }
};

int main() {
    VectorStorage storage; // 创建VectorStorage对象用于存储向量
    // 添加三个向量到存储中
    storage.addVector(VectorData({1.0, 2.0, 3.0}, "Vector 1", "First vector in storage", std::time(0) - 10));
    storage.addVector(VectorData({4.0, 5.0, 6.0}, "Vector 2", "Second vector in storage", std::time(0) - 5));
    storage.addVector(VectorData({7.0, 8.0, 9.0}, "Vector 1", "Third vector in storage", std::time(0) - 5));

    std::cout << "Stored vectors after adding:\n";
    storage.printAll(); // 打印存储中的所有向量

    std::cout << "Multi-dimensional query by label 'Vector 1' and timestamp range:\n";
    auto result1 = storage.queryVectors("Vector 1", "", std::time(0) - 10, std::time(0)); // 同时通过标签和时间戳范围查询
    for (const VectorData& vecData : result1) vecData.print();
    if (!result1.empty()) {
        std::cout << "Test 1 passed!\n";
    } else {
        std::cout << "Test 1 failed!\n";
    }

    std::cout << "Multi-dimensional query by label 'Vector 1', description 'Third vector in storage' and timestamp range:\n";
    auto result2 = storage.queryVectors("Vector 1", "Third vector in storage", std::time(0) - 10, std::time(0)); // 同时通过标签、描述和时间戳范围查询
    for (const VectorData& vecData : result2) vecData.print();
    if (!result2.empty()) {
        std::cout << "Test 2 passed!\n";
    } else {
        std::cout << "Test 2 failed!\n";
    }

    return 0; // 主函数返回0，表示程序正常结束
}





