addi $t0, $zero, 44
addi $t1, $zero, 16
add $t2, $t0, $t1
jalr $t0
addi $t1,$zero,17
addi $t0, $t0, 1
addi $t0, $t0, 1
addi $t0, $t0, 1
addi $t0, $t0, 1
addi $t0, $t0, 1
addi $t0, $t0, 1

label:
	addi $t7, $zero, 32
	jr $ra
