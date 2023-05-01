## RISC-V simulator(single cycle program)을 구현하는 프로젝트

#### 개발 환경 : LINUX
<br/><br/> 
> Instruction Fetch(IF) 단계: instruction 메모리 상의 명령어를 읽는 단계 <br/><br/>
Instruction Decode(ID) 단계 : 가져온 instruction을 해석하는 단계 <br/><br/> 
Execute(EX) 단계            : 해석된 instruction을 바탕으로 계산하는 단계 <br/><br/> 
Memory(MEM) 단계            : 데이터 메모리에 접근하는 단계 <br/><br/> 
Write back(WB) 단계         : 메모리에 저장된 값을 읽어오는 단계<br/><br/> 

<br/><br/> 

## ✔ add <br/><br/> 
add는 R-type의 연산자로, 레지스터값 두 개(rs1, rs2)를 더하여 또 다른 레지스터(rd)에 저장하는 방법의 연산이다. add의 opcode는 0110011이고, 이를 10진수로 나타내면 51이다. 따라서 opcode가 51이라면 EX에서 add 연산을 수행하도록 한다. Instruction에서 각 비트 별로 나눠서 위치에 맞춰서 피연산자 rs1과 rs2를 받아오고(ID), 이 받아온 레지스터들을 EX 단계에서 둘이 더해준 후 temp에 저장을 해준다(EX). 이후, temp에 저장을 해준 연산 결과를 write back 단계에서 또 다른 레지스터(rd)에 저장을 해준다(WB). 이때, add 연산에서는 메모리에 대한 접근이 불필요하다. 또한, x0은 레지스터 상에서 상수 0으로 고정되어있기 때문에 x0에는 값이 들어가면 안된다. 따라서 결과 값을 저장할 레지스터(rd)가 0이 아닌 경우에만 이러한 연산이 실행되고, 0인 경우에는 연산값이 저장되지 않고 0이 된다. 아래 그림들은 add 연산에서 수행되는 ID, EX, WB 과정을 나타낸 코드이다.
<br/><br/> 

<img width="800" alt="image" src="https://user-images.githubusercontent.com/110325367/235458846-1e51a217-1c89-4c80-b10c-82e5d154ee2e.png">
<br/><br/> 


## ✔ addi <br/><br/> 
addi는 I-type의 연산자로, 레지스터 값 하나(rs1)와 immediate 값을 더하여 또 다른 레지스터(rd)에 저장하는 방법의 연산이다. addi의 opcode는 0010011이고, 이를 10진수로 나타내면 19이다. 따라서 opcode가 19라면 EX에서 addi 연산을 수행하도록 한다. 우선 instruction에서 각 비트 별로 나눠서 위치에 맞춰서 피연산자 rs1과 상수 값 immediate를 받아온다, 이때 받아오는 상수 값은 양수가 될 수도 있고 음수가 될 수도 있는데, 만약 받아온 상수 값이 양수일 경우에는 임시 저장소인 tfi에 바로 저장을 해주고, 받아온 상수 값이 음수일 경우, 보수 처리 방식을 이용하여 연산해야 하는데, 0은 1로, 1은 0으로 바꿔준 후, 1을 더해주고, 이 값에 마이너스 부호를 붙여서 임시 저장소인 tfi에 넣어준다(ID). 이러한 tfi값과 rs1을 더하여 temp에 잠시 저장해준다(EX). 이후, temp에 저장된 연산 결과를 write back 단계에서 또 다른 레지스터(rd)에 저장을 해준다(WB). 이때, addi 연산에서도 add 연산에서와 마찬가지로 메모리에 대한 접근이 불필요하며, 값을 저장할 레지스터(rd)가 0이 아니어야 한다. 아래 그림들은 addi 연산에서 수행되는 ID, EX, WB 과정을 나타낸 코드이다.
<br/><br/> 

<img width="800" alt="image" src="https://user-images.githubusercontent.com/110325367/235458950-5ca9a744-e15a-4470-b311-b493eaf9f304.png"><br/><br/> 

<img width="800" alt="image" src="https://user-images.githubusercontent.com/110325367/235459023-c4afbf8d-573a-45ae-8d52-8bd6fde82e76.png">
<br/><br/> 


## ✔ ld <br/><br/> 
ld는 I-type의 연산자로, 메모리에 저장된 값을 레지스터(rs1)에서 immediate에 가져와서 읽고, 계산 값을 또 다른 레지스터(rd)에 넣는 방식의 연산이다. 주로 sd와 함께 쓰이는 instruction이다. ld의 opcode는 0000011이고, 이를 10진수로 나타내면 3이다. 따라서 opcode가 3라면 EX에서 ld 연산을 수행하도록 한다. 우선 instruction에서 각 비트 별로 나눠서 위치에 맞춰서 피연산자 rs1과 상수 값 immediate를 받아온다, 만약 받아온 상수 값이 양수일 경우, tfi에 상수 값을 바로 저장해주고, 받아온 상수 값이 음수일 경우, 0은 1로, 1은 0으로 바꿔준 후 1을 더해주고, 이 값에 마이너스 부호를 붙인 값을 tfi에 저장해줘야 한다(ID). 이렇게 저장된 tfi를 rs1과 더하여 그 값을 temp에 저장해둔다(EX). 이후, temp에 저장된 결과를 data_mem에 넣고 이 값을 다시 temp로 지정한 후(MEM), 다른 레지스터(rd)에 이 값을 넣어야 한다. 아래 그림들은 ld 연산에서 수행되는 ID, EX, MEM, WB 과정을 나타낸 코드이다.
<img width="800" alt="image" src="https://user-images.githubusercontent.com/110325367/235459099-e93b94a3-3117-47ef-9e8f-11c4be6fa535.png">
<br/><br/> 


## ✔ sd <br/><br/> 
sd는 S-type의 연산자로, ld와 반대로 레지스터에 저장된 값을 메모리에 저장하는 방식의 연산이다. sd의 opcode는 0100011이고, 이를 10진수로 나타내면 35이다. 따라서 opcode가 35라면 EX에서 sd 연산을 수행하도록 한다. 우선 instruction에서 각 비트 별로 나눠서 위치에 맞춰서 피연산자 rs1과 rs2, 그리고 상수 
값 immediate를 받아온다(ID), 만약 받아온 상수 값이 양수일 경우, tfi에 상수 값을 바로 저장해주고, 받아온 상수 값이 음수일 경우, 0은 1로, 1은 0으로 바꿔준 후 1을 더해주고, 이 값에 마이너스 부호를 붙인 값을 tfi에 저장해줘야 한다(ID). 이렇게 저장된 tfi를 rs1과 더하여 그 값을 temp에 저장해둔다(EX). 이후, temp에 저장된 결과를 data_mem에 넣고 레지스터 값(rs2)을 이 값을 넣어야 한다. sd는 메모리에 저장하는 것이기 때문에 WB 과정을 필요로 하지 않는다. 아래 그림들은 ld 연산에서 수행되는 ID, EX, MEM 과정을 나타낸 코드이다.
<br/><br/> 

<img width="800" alt="image" src="https://user-images.githubusercontent.com/110325367/235459177-45a7c7a4-4573-4bc5-a562-d095175dae0d.png">
<br/><br/> 


## ✔ beq <br/><br/> 
beq는 SB-type의 연산자로, rs1과 rs2가 같다면 다음 pc가 pc+4의 위치가 아니라 pc+immediate의 위치로 이동하는 연산자이다. beq의 opcode는 1100011이고, 이를 10진수로 나타내면 99이다. 따라서 opcode가 99라면 EX에서 beq 연산을 수행하도록 한다. 우선 instruction에서 각 비트 별로 나눠서 위치에 맞춰서 피연산자 rs1과 rs2, 그리고 상수 값 immediate를 받아온다. immediate를 받아올 때, pc 점프는 짝수번째로만 가능하기 때문에, 32bit를 64bit로 바꿔주기 위해서 한비트씩 왼쪽으로 이동, 즉 곱하기 2를 해준다. (ID), 받아온 후 rs2의 값과 rs1의 값이 같은지 다른지를 확인하기 위해 둘을 빼준다. 둘을 뺐을 때의 값이 0이라면 두 레지스터 안의 값은 같기 때문에 pc+immediate를 하고, 이 값이 다르다면 pc는 원래대로 pc+4를 해줘야 한다. 여기서 주의할 점은, 현재의 pc를 이용하여 비교해야하기 때문에, 다음 위치를 가리키는 pc가 아닌, pc-4로 해야 한다(EX). beq에서는 값을 저장하는 것이 아닌 pc의 위치를 변경하는 것이므로, MEM, WB 단계가 불필요하다. 아래 그림들은 beq 연산에서 수행되는 ID, EX 과정을 나타낸 코드이다.
<br/><br/> 

<img width="800" alt="image" src="https://user-images.githubusercontent.com/110325367/235459217-b6703233-bbc6-4aba-89c1-9a0a550467b9.png">
<br/><br/> 


## ✔ jal <br/><br/> 
jal은 UJ-type의 연산자로, x1에 돌아올 pc값, 즉 현재 위치를 저장한 후 pc+immediate의 위치로 이동하는 연산자이다. jal의 opcode는 1101111이고, 이를 10진수로 나타내면 111이다. 따라서 opcode가 111라면 EX에서 jal 연산을 수행하도록 한다. instruction에서 각 비트 별로 나눠서 위치에 맞춰서 피연산자 rs1과 상수 값 immediate를 받아온다. immediate를 받아올 때, pc 점프는 beq과 마찬가지로 짝수 번째로만 가능하므로, 32bit를 64bit로 바꿔주기 위해서 한비트씩 왼쪽으로 이동, 즉 곱하기 2를 해준다(ID), 이후, pc 값을 rd의 위치에 저장해주고, 이동하고자 하는 주소, 즉 pc+immediate로 위치를 이동한다. 여기서 주의할 점은, 현재의 pc 정보를 이용해야하기 때문에, 다음 위치를 가리키는 pc가 아닌, pc-4를 한 번 해주어야 한다(EX). jal에서도 beq에서와 마찬가지로 특정 값을 저장하는 것이 아닌 pc의 위치를 변경하여 이동하는 것이므로, MEM, WB 단계가 불필요하다. 아래 그림들은 jal 연산에서 수행되는 ID, EX 과정을 나타낸 코드이다.
<br/><br/> 

<img width="800" alt="image" src="https://user-images.githubusercontent.com/110325367/235459263-6a2cedb6-5de6-417f-9995-caeaf1e5ed66.png">
<br/><br/> 


## ✔ jalr <br/><br/> 
jalr은 I-type의 연산자로, x1에 돌아올 pc값, 즉 현재 위치를 저장한 후 rs1+immediate의 위치로 이동하는 연산자이다. jalr의 opcode는 1100111이고, 이를 10진수로 나타내면 103이다. 따라서 opcode가 103이라면 EX에서 jalr 연산을 수행하도록 한다. instruction에서 각 비트 별로 나눠서 위치에 맞춰서 피연산자 rs1과 상수 값 immediate를 받아온다. immediate를 받아올 때, pc 점프는 beq이나 jal과 마찬가지로 짝수 번째로만 가능하므로, 32bit를 64bit로 바꿔주어야 한다. 그러므로 한비트씩 왼쪽으로 이동, 즉 곱하기 2를 해준다(ID), 이후, pc 값을 rd의 위치에 저장해주고, 이동하고자 하는 주소, 즉 rs1+immediate로 위치를 이동한다. 여기서 주의할 점은, 현재의 pc 정보를 이용해야하기 때문에, 다음 위치를 가리키는 pc가 아닌, pc-4를 한 번 해주어야 한다(EX). 또, rd 값이 0인경우는 다시 말하면 레지스터 x0를 뜻하는 경우인데, x0는 무조건 0이어야하기 때문에, 값이 들어가지 않도록 해야한다. jalr에서도 jal이나 beq에서와 마찬가지로 특정 값을 저장하는 것이 아닌 pc의 위치를 변경하여 이동하는 것이므로, MEM, WB 단계가 불필요하다. 아래 그림들은 jalr 연산에서 수행되는 ID, EX 과정을 나타낸 코드이다.
<br/><br/> 

<img width="800" alt="image" src="https://user-images.githubusercontent.com/110325367/235459301-4608dee1-90bf-4f0d-a1df-b685f50b4057.png">






