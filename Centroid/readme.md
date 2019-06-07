## 폴리곤의 중점 구하기

--------------



폴리곤의 각 정점이 아래와 같을 때 폴리곤 모양은 다음과 같다.

{139, 106},{142, 190}, {290, 220},{382, 141},{360, 63},{287, 115},{210, 88},{200, 140}

![1559857699794](https://github.com/rlatkddn212/geomertyAlgorithm/blob/master/assets/1559857699794.png)

이런 폴리곤에서 중점을 구하려면

![C_{\mathrm {x} }={\frac {1}{6A}}\sum _{i=0}^{n-1}(x_{i}+x_{i+1})(x_{i}\ y_{i+1}-x_{i+1}\ y_{i})](https://wikimedia.org/api/rest_v1/media/math/render/svg/288871268387a710a3bd9a2227b14aa89b18247e)

![C_{\mathrm {y} }={\frac {1}{6A}}\sum _{i=0}^{n-1}(y_{i}+y_{i+1})(x_{i}\ y_{i+1}-x_{i+1}\ y_{i})](https://wikimedia.org/api/rest_v1/media/math/render/svg/0b415b79720a8eb41ee9d77da1c72f66393336b8)