class Table:
    def __init__(self, n, m):
        self.tab = [[0 for i in range(0, m)] for j in range(0, n)]
    
    def __str__(self):
        s = []
        for row in self.tab:
            s.append(str(row))
        return "\n".join(s)

    def __getitem__(self, tup):
        i, j = tup
        return self.tab[i][j]

    def __setitem__(self, tup, item):
        i, j = tup
        self.tab[i][j] = item

    def __iter__(self):
        return iter(self.tab)
        

def LCS(x, y) -> int:
    n = len(x)
    m = len(y)
    L = Table(n, m)
    for i in range(0, n):
        for j in range(0, m):
            if x[i] == y[j]:
                L[i, j] = L[i-1, j-1] + 1
            else:
                L[i, j] = max(L[i-1, j], L[i, j-1])
    return L[n-1, m-1]


def knapsack(items: list[tuple[int, int]], capacity: int):
    W = [None] + [tup[0] for tup in items]
    V = [None] + [tup[1] for tup in items]
    B = Table(len(items) + 1, capacity + 1) 
    keep = Table(len(items) + 1, capacity + 1) 
    for i in range(1, len(items) + 1):
        current_item = items[i - 1]
        print("Current item:", current_item)
        for j in range(1, capacity + 1):
            new_value = V[i] + B[i-1, j - W[i]]
            if (W[i] < j) or (new_value <= B[i-1, j]):
                B[i, j] = B[i-1, j]
                keep[i, j] = False
            else:
                B[i, j] = new_value
                keep[i, j] = True
            print(B, "\n")
    print(B, keep)

if __name__ == "__main__":
    #print(Table(10, 12))
    #assert(LCS(y="CGATAATTGAGA", x="GTTCCTAATA") == 10)
    #assert(LCS(y="RELATIVELY", x="ACTIVE") == 5)
    #print("passed")
    knapsack([(3, 6), (2, 5), (4, 7), (1, 3)], 8)
