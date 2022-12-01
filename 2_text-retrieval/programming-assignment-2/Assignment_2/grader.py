from __future__ import division


def ap(relevant_docs, user_docs):
    '''
    Calculates the AP for two given lists
    of relevant document IDs and user-supplied
    document IDs
    '''
    precision_sum = 0
    relevant_num = 0
    for i in range(len(user_docs)):
        if user_docs[i] in relevant_docs:
            relevant_num += 1
            precision_sum += relevant_num / (i + 1)

    return precision_sum / len(relevant_docs)


def precisionat5(relevant_docs, user_docs):
    precision_sum = 0
    for i in range(min(len(user_docs), 5)):
        if user_docs[i] in relevant_docs:
            precision_sum += 1

    return precision_sum / min(len(user_docs), 5)


def precisionat5avg(relevant, user):
    pre = 0
    for i in range(len(user)):
        pre += precisionat5(relevant[i], user[i])
    return pre / len(user)


def Map(relevant, user):
    '''
    Given two list of lists containing all the relevant
    documents for all the queries, return the map
    '''
    map = 0
    for i in range(len(user)):
        map += ap(relevant[i], user[i])

    return map / len(relevant)


def gmap(relevant, user):
    '''
    Given two list of lists containing all the relevant
    documents for all the queries, return the gmap
    '''
    gmap = 1
    for i in range(len(user)):
        gmap = gmap * ap(relevant[i], user[i])

    return gmap ** (1 / len(relevant))


submission = open('relevantdocs.txt').read()

usersubmission = submission.split('\n')
usersubmission = [line.rstrip() for line in usersubmission]
if(usersubmission[-1] == ''):
    usersubmission.pop()
newsubmission = []
for item in usersubmission:
    newsubmission.append(item.split(' '))


newsubmission = [[float(x) for x in l] for l in newsubmission]


submission1 = open('build/Assignment2/output.txt').read()

usersubmission1 = submission1.split('\n')
usersubmission1 = [line.rstrip() for line in usersubmission1]
if(usersubmission1[-1] == ''):
    usersubmission1.pop()
usersubmission1.pop(0)
newsubmission1 = []
for item in usersubmission1:
    newsubmission1.append(item.split())

newsubmission1 = [[float(x) for x in l] for l in newsubmission1]

fout = open("submit.txt", 'w')
fout.write(str(Map(newsubmission, newsubmission1)))
