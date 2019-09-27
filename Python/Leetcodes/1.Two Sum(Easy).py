
# LeetCode:1.Two Sum(Easy)

'''
Given an array of integers, return indices of the two numbers such that they add up to a specific target.

You may assume that each input would have exactly one solution, and you may not use the same element twice.

Example:

Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,

return [0, 1].
'''
# My Code:

array = []

while True:
    n = input("Input an integer, if nothing, then quit:")
    if not n.isdigit():
        break
    array.append(int(n))
print('Given nums =',array)

while True:
    target = input("Input the target, if nothing, then quit:")
    if target.isdigit():
        print('Target is ' + target)
        target = int(target)
        break

output = []
for i in range(len(array)):
    j = target - array[i]
    if j in array and array.index(j) > i:
        output.append([i,j])
print(output)
