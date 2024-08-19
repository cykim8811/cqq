import tensornetwork as tn
import numpy as np

N = 4

q = [tn.Node(np.array([1, 0], dtype=np.complex32), name='q' + str(i)) for i in range(N)]


def x(target):
    if type(target) is not list:
        xnode = tn.Node(np.array([[0, 1], [1, 0]], dtype=np.complex32), name='x')
        target ^ xnode[0]
        return xnode[1]
    else:
        result = []
        for t in target:
            result.append(x(t))
        return result

def cx(control, target):
    cxtensor = np.zeros((2, 2, 2, 2), dtype=np.complex32)
    cxtensor[0, 0, 0, 0] = 1
    cxtensor[0, 1, 0, 1] = 1
    cxtensor[1, 0, 1, 1] = 1
    cxtensor[1, 1, 1, 0] = 1
    cxnode = tn.Node(cxtensor, name='cx')
    control ^ cxnode[0]
    target ^ cxnode[1]
    return cxnode[2], cxnode[3]


# Implementation of increment circuit

# anc = tn.Node(np.array([0, 1], dtype=np.complex32), name='anc')

# for i in range(N):
#     cx(anc, q[i])
#     x([q[k] for k in range(0, i)])
#     mcx([q[k] for k in range(0, i+1)], anc)
#     x([q[k] for k in range(0, i)])

# cx(anc, q[N-1])
# x(anc)
# x([q[k] for k in range(0, N-1)])
# mcx([q[k] for k in range(0, N-1)], anc)
# x([q[k] for k in range(0, N-1)])

