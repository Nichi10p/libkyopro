import sys
from subprocess import check_output
from math       import comb
from random     import choices

def gen():
  # MOD = 998244353
  MOD = 31
  N = 8000
  Q = 10**4
  R = choices(range(N+1), k=Q)
  _nc = [comb(N, i) % MOD for i in range(N+1)]
  ans = [_nc[r] for r in R]
  with open('_in.txt', 'w') as f:
    print(MOD, N, max(R), Q, file=f)
    print(*R, file=f)
  with open('_out.txt', 'w') as f:
    print(*ans, file=f)
    pass

def test():
  with open('_in.txt') as f:
    MOD, N, maxR, Q = map(int, f.readline().split())
    R = [int(it) for it in f.readline().split()]
  with open('_out.txt') as f:
    ans = [int(it) for it in f.readline().split()]
    pass
  for _ in range(30):
    with open('_in.txt') as f:
      out = check_output(['./a.out'], stdin=f, stderr=sys.stderr, text=True, timeout=5)
      out = [int(o) for o in out.split()]
      mis = [i for i in range(Q) if ans[i] != out[i]]
      if mis:
        print('Wrong Answer')
        for i in mis:
          print(f'comb({N}, {R[i]}) % {MOD} -> {out[i]}, (expected {ans[i]})')
        return
  print('Accepted')

if __name__ == '__main__':
  gen()
  test()
