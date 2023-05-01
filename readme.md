## RISC-V simulator(single cycle program)을 구현하는 프로젝트

#### 개발 환경 : LINUX
<br/><br/> 
> single cycle은 크게 다섯 단계의 과정을 거치면서 진행이 되는데, instruction 메모리 상의 명령어를 읽는 Instruction Fetch(IF) 단계, 가져온 instruction을 해석하는 Instruction Decode(ID) 단계, 해석된 instruction을 바탕으로 계산을 하는 Execute(EX) 단계, 데이터 메모리에 접근하는 Memory(MEM) 단계, 메모리에 저장된 값을 읽어오는 Write back(WB) 단계로 분류된다. 싱글 사이클을 통해 많은 instruction을 수행할 수 있지만, 그 중에서 대표적인 몇 가지를 뽑아서 구현하고자 하였다. R-type에 속하는 add, I-type에 속하는 addi, ld, jalr, S-type에 속하는 sd, SB-type에 속하는 beq, UJ-type에 속하는 jal을 구현하였다.

