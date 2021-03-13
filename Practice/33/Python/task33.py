prt = print

def create(length, a0, step):
    arr = [a0]
    for i in range(1,length):
        arr.append(arr[i-1] + step)
    return arr

def sort(arr, length=-1):
    if length == -1:
        length = len(arr)
    for i in range(1,length):
        for j in range(i, 0, -1):
            if (arr[j-1] > arr[j]): arr[j-1], arr[j] = arr[j], arr[j-1]
            else: break
    return arr

def print(arr, length=-1):
    if length == -1:
        length = len(arr)
    prt('[', end='')
    for i in range(length-1):
        prt(arr[i], ', ', sep='', end='')
    if length > 0: prt(arr[length-1], end='')
    prt(']')

length = int(input())
a0 = int(input())
step = int(input())

arr = create(length, a0, step)
arr = sort(arr)
print(arr)
