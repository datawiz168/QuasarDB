import random
from typing import List

class LSH:
    def __init__(self, num_tables: int, dim: int):
        # 初始化LSH对象
        self.num_tables = num_tables  # 定义哈希表的数量
        self.dim = dim  # 定义向量的维度
        self.tables = [{} for _ in range(num_tables)]  # 初始化哈希表，每个表都是一个字典
        # 初始化随机超平面，每个超平面是一个随机向量
        self.hyperplanes = [[random.gauss(0, 1) for _ in range(dim)] for _ in range(num_tables)]

    def hash_vector(self, vector: List[float], table_index: int) -> int:
        # 计算给定向量在指定哈希表的哈希值
        # 使用点积和随机超平面计算哈希值
        return sum(v * h for v, h in zip(vector, self.hyperplanes[table_index])) > 0

    def insert_vector(self, vector: List[float], index: int):
        # 将向量插入所有哈希表
        for i in range(self.num_tables):
            h = self.hash_vector(vector, i)  # 计算哈希值
            if h not in self.tables[i]:  # 如果桶不存在，创建新桶
                self.tables[i][h] = []
            self.tables[i][h].append(index)  # 将向量索引添加到桶中

    def query_vector(self, vector: List[float]) -> List[int]:
        # 查询向量并返回匹配的索引
        result_set = set()  # 使用集合避免重复
        for i in range(self.num_tables):
            h = self.hash_vector(vector, i)  # 计算哈希值
            if h in self.tables[i]:  # 如果桶存在，将其内容添加到结果集
                result_set.update(self.tables[i][h])
        return list(result_set)  # 返回结果列表

class MultiLayerLSH:
    def __init__(self, num_layers: int, num_tables: int, dim: int):
        # 初始化多层LSH对象
        # 每层都是一个独立的LSH对象
        self.lsh_layers = [LSH(num_tables, dim) for _ in range(num_layers)]

    def insert_vector(self, vector: List[float], index: int):
        # 将向量插入每一层的LSH
        for lsh in self.lsh_layers:
            lsh.insert_vector(vector, index)

    def query_vector(self, vector: List[float]) -> List[int]:
        # 查询向量并返回匹配的索引
        result_set = set()  # 使用集合避免重复
        for lsh in self.lsh_layers:  # 遍历每一层LSH
            result_set.update(lsh.query_vector(vector))  # 将查询结果添加到结果集
        return list(result_set)  # 返回结果列表
