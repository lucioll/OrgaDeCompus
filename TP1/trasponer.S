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


    inicializacion:     
		add     t2, zero, zero  # Indice i
		add     t5, zero, a3   # inicializo direccion de salida en t5

	fori:   
        addi   t3, zero, 0  # Indice j
        beq    t2, a1, fin  # Fin
        b forj            # Vamos a forj
	
	aumentari:
		addi t2, t2, 1  # i++
		b fori		#vamos a fori
		
	forj:
        mulou  t9, t3, a1   #(j * columna)
        add    t9, t2, t9   #(i + (j * columna))
        sll    t9, t9, 3    #(i + (j * columna)) * 8
        add    t9, a2, t9   #direccion de entrada + [(i + (j * columna)) * 8]
        lw     t6, 0(t9)    #guardo valor de entrada en t6 y t7 por ser double word
		lw     t7, 4(t9)
		
        sw      t6, 0(t5)
        sw      t7, 4(t5)		#guardo valor de salida en la direccion que almacena t5
        addi    t5, t5, 8      #pos++
        addi    t3, t3, 1      #j++
        beq     t3, a0, aumentari   #vamos a aumentari
		b forj		#vamos a forj

	fin:	/* PREPARE TO DELETE STACK */
    	lw gp, trasponer_frame_gp(sp)
    	lw $fp, trasponer_frame_fp(sp)
    	lw ra, trasponer_frame_ra(sp)
    	addu sp, sp, trasponer_frame_size
    	jr ra

    .end trasponer


