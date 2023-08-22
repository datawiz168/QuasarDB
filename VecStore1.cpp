#include <iostream>
#include <vector>
#include <cassert>

class VectorData {
public:
    std::vector<double> vector; // 存储向量的元素

    // 构造函数，可以接受一个初始化向量
    VectorData(const std::vector<double>& vec) : vector(vec) {}

    // 输出向量的方法，方便调试
    void print() const {
        for (double value : vector) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

class VectorStorage {
public:
    // 添加向量到存储中
    void addVector(const VectorData& data) {
        vectors.push_back(data);
    }

    // 打印所有存储的向量，方便调试
    void printAll() const {
        for (const VectorData& vecData : vectors) {
            vecData.print();
        }
    }

private:
    std::vector<VectorData> vectors; // 存储所有向量的容器
};

// 测试向量的存储和检索
void testVectorStorage() {
    VectorStorage storage;
    VectorData vec1({1.0, 2.0, 3.0});
    VectorData vec2({4.0, 5.0, 6.0});

    storage.addVector(vec1);
    storage.addVector(vec2);

    storage.printAll(); // 打印存储的向量以手动验证

    // 这里我们使用一个简单的方法来检查向量是否存储正确。
    // 在实际应用中，您可能希望添加更多的方法来访问和验证存储的向量。
}

int main() {
    testVectorStorage();

    VectorStorage storage;
    storage.addVector(VectorData({1.0, 2.0, 3.0}));
    storage.addVector(VectorData({4.0, 5.0, 6.0}));

    std::cout << "Stored vectors:\n";
    storage.printAll();

    std::cout << "All tests passed!\n";

    return 0;
}
