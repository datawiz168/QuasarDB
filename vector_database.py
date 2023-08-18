from lsh import MultiLayerLSH
from typing import List, Any
import pickle

class VectorDatabase:
    def __init__(self, num_layers: int = 2, num_tables: int = 3, dim: int = 2):
        # 初始化向量数据库
        self.lsh = MultiLayerLSH(num_layers, num_tables, dim)  # 创建多层LSH实例
        self.vectors = []  # 存储插入的向量
        self.ids = []  # 存储向量的ID

    def insert_vector(self, vector: List[float], vector_id: Any = None):
        # 插入向量到数据库
        index = len(self.vectors)  # 获取新向量的索引
        self.vectors.append(vector)  # 将向量添加到向量列表
        self.ids.append(vector_id)  # 将向量ID添加到ID列表
        self.lsh.insert_vector(vector, index)  # 使用LSH插入向量

    def query(self, vector: List[float], num_results: int = 1) -> List[int]:
        # 查询向量并返回最相似的结果索引
        indices = self.lsh.query_vector(vector)  # 使用LSH查询向量
        return indices[:num_results]  # 返回前num_results个结果

    def save_to_file(self, file_path: str):
        # 将数据库保存到文件
        with open(file_path, 'wb') as file:
            pickle.dump(self, file)  # 使用pickle序列化对象并保存到文件

    @staticmethod
    def load_from_file(file_path: str):
        # 从文件加载数据库
        with open(file_path, 'rb') as file:
            return pickle.load(file)  # 使用pickle加载序列化的对象

