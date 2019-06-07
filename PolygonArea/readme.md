## 폴리곤 넓이 계산

---------------------------



폴리곤은 안의 임의의 점에서 각 정점들을 연결하여 삼각형을 만든 후 합을 구한다.

![1559773957688](https://github.com/rlatkddn212/geomertyAlgorithm/blob/master/assets/1559773957688.png)

s = s1 + s2 + s3 +s4 + s5 + s6

로 계산된다. 각 삼각형의 넓이는 외적을 통해 계산가능하다.



![A={\frac {1}{2}}\sum _{i=0}^{n-1}(x_{i}\ y_{i+1}-x_{i+1}\ y_{i})\;](https://wikimedia.org/api/rest_v1/media/math/render/svg/d95df1d231c51a86daecc738904641b8c3fae7e3).



임의의 점을 삼각형 외부로 두고 계산하거나 오목 폴리곤을 처리하는 경우에도 사용가능하다.

외적을 사용하면 폴리곤 외부에 만들어진 삼각형의 경우 음수 넓이가 나오기 때문에 자동적으로 감해진다.


