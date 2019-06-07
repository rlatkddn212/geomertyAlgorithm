## barycentric coordinates

-------------



지형에서 고도 값을 구한다고 해보자. 지형은 삼각형들로 이루어져 있기 때문에 삼각형 위에 점의 고도 값을 구해야한다.

barycentric coordinates는 삼각형 안에서 한점에서 적절히 보간된 속성값을 구하는 방법으로 사용된다.

더 예를 들어보면 삼각형 각 정점에 texture 좌표나 normal 값 같은 속성 값들이 저장된다. 레스터라이즈될 때 이 값들이 한 픽셀마다 그 속성 값들이 구해져야하는데 barycentric coordinates를 사용해서 구할 수 있다.



![1559861296821](C:\Users\swkim\AppData\Roaming\Typora\typora-user-images\1559861296821.png)

위 삼각형의 점 P에서의 속성값을 구한다고 해보자.

각 삼각형의 넓이 비율을 사용하여 구할 수 있다.

삼각형 ABP의 넓이는 C값에 비율

삼각형 ACP의 넓이는 B값의 비율

삼각형 BCP의 넓이는 A값의 비율

로 볼 수 있다.



즉 점 p에서의 속성 값을 

p.a = (area(ABP) / area(ABC)) * C.a  + (area(ACP) / area(ABC)) * B.a + (area(BCP) / area(ABC)) * A.a

로 계산되어진다.