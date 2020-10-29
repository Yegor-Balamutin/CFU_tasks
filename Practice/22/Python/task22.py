def fill(i,j):
    d[i][j] = False
    if (d[i+1][j]):
        i1 = i + 1
        j1 = j
        check(i1,j1)
    if (d[i][j+1]):
        i1 = i
        j1 = j + 1
        check(i1,j1)
    if (d[i-1][j]):
        i1 = i - 1
        j1 = j
        check(i1,j1)
    if (d[i][j-1]):
        i1 = i
        j1 = j - 1
        check(i1,j1)

def check(i1,j1):
    if (maze[i1][j1] == "#"):
        None
    elif (maze[i1][j1] == " "):
        fill(i1, j1)
    else:
        exits[maze[i1][j1]] = True

maze = [                             # 25 str * 27 chr
	"####B######################",
    "# #       #   #      #    #",
    "# # # ###### #### ####### #",
    "# # # #       #   #       #",
    "#   # # ######### # ##### #",
    "# # # #   #       #     # #",
    "### ### ### ############# #",
    "# #   #     # #           #",
    "# # #   ####### ###########",
    "# # # #       # #         C",
    "# # ##### ### # # ####### #",
    "# # #     ### # #       # #",
    "#   ##### ### # ######### #",
    "######### ### #           #",
    "# ####### ### #############",
    "A           #   ###   #   #",
    "# ############# ### # # # #",
    "# ###       # # ### # # # #",
    "# ######### # # ### # # # F",
    "#       ### # #     # # # #",
    "# ######### # ##### # # # #",
    "# #######   #       #   # #",
    "# ####### ######### #######",
    "#         #########       #",
    "#######E############D######"
]
d = [[True]*29]*27
for i in range(25):   # 2 последних доп символа каждой строки = False
    d[i][27] = False
    d[i][28] = False
for i in range(29):   # 2 последних доп строки = False
    d[25][i] = False
    d[26][i] = False
exits = {'A': False, 'B': False, 'C': False, 'D': False, 'E': False, 'F': False}
i,j = input("Координаты: ").split(" ")
i = int(i)
j = int(j)
if (i < 0) or (i > 24) or (j < 0) or (j > 26) or (maze[i][j] == "#"):
    print("Неверные координаты")
else:
    if (maze[i][j] >= 'A') and (maze[i][j] <= 'F'):
        exits[maze[i][j]] = True
    fill(i,j)
    print("Доступные выходы: ")
    a = ord('A')
    while (a <= ord('F')):
        if (exits[chr(a)]):
            print(chr(a))
        a += 1
input("Нажмите Enter")