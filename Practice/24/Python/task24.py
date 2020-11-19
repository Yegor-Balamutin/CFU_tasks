import json
f = open('in.json', 'r')
file1 = json.load(f)
f.close()
users_tasks = {}
for i in file1:
	id = i['userId']
	if not(users_tasks.get(id)):
		users_tasks[id] = 0
	if (i['completed']):
		users_tasks[id] += 1
file2 = json
file2 = []
for i in users_tasks.items():
	file2.append({"userId": i[0], "task_completed": i[1]})
f = open('out.json', 'w')
json.dump(file2, f, indent = 2)
f.close()
input("ok")
