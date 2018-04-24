#include <mips/regdef.h>

.globl trasponer

.text
.abicalls
.align 2
.ent trasponer


trasponer:

	#define trasponer_frame_size 48
    #define trasponer_frame_ra 40
    #define trasponer_frame_fp 36
    #define trasponer_frame_gp 32
    #define trasponer_frame_4arg 60  /* Direccion base de la matriz traspuesta */
    #define trasponer_frame_3arg 56  /* Direccion base de la matriz original */
    #define trasponer_frame_2arg 52  /* Cantidad de columnas */
    #define trasponer_frame_1arg 48  /* Cantidad de filas */
    
    .frame $fp, trasponer_frame_size, ra

    /* Creando el stack frame */

    subu sp, sp, trasponer_frame_size
    .cprestore 16 	/* Restaura el valor original del sp */
    sw gp, trasponer_frame_gp(sp)
    sw $fp, trasponer_frame_fp(sp)
    sw ra, trasponer_frame_ra(sp)
    move  $fp, sp 	/* inserto el valor del stack pointer en el frame pointer */
      
    sw a0, trasponer_frame_1arg($fp) /* Guardamos el primer argumento (unsigned int filas) */
    sw a1, trasponer_frame_2arg($fp) /* Guardamos el segundo argumento (unsigned int columnas) */
    sw a2, trasponer_frame_3arg($fp) /* Guardamos el tercer argumento (long long *entrada) */
    sw a3, trasponer_frame_4arg($fp) /* Guardamos el cuarto argumento (long long *salida) */

    lw    t0, a2                 # $t0 represents start address of entrada[i][j]
	lw    t1, a3                 # $t1 represents start address of salida[i][j]

    inicializacion:     add     t2, zero, zero  # Indice i
    					add     t4, zero, a1    # Maxima cantidad de iteraciones = cant columnas

	fori:   addi   t3, zero, 0  	 # Indice j
			jal    loopj             # Vamos al loopj
			addi   t2, t2, 1       	 # i++
			bne    t4, t2, forj      # Continuamos mientras i < columna
			j fin
			
	forj:	sll    t7, t2, 4       
			add    t7, t7, t3
			sll    t7, t7, 2      # 4 * ((i * columna) + j)  
			add    t9, t7, t0     # direccion de entrada[i][j]
			lw     t9, 0(t9)      # valor de entrada[i][j] i+j*columna

			sll    t5, t3, 4       
			add    t5, t5, t2
			sll    t5, t5, 2      # 4 * ((j * columna) + i)
			add    t5, t5, t1     # direccion de salida[j][i]
			sw     t5, 0(t9)      # guardo en salida[j][i] lo que saque en la entrada

			addi   t3, t3, 1      # j++
			bne    t3, a0, forj   # continuamos en forj if j < fila
	
	fin:	/* PREPARE TO DELETE STACK */
    		lw gp, trasponer_frame_gp(sp)
    		lw $fp, trasponer_frame_fp(sp)
    		lw ra, trasponer_frame_ra(sp)
    		addu sp, sp, trasponer_frame_size
    		jr ra

    .end trasponer


