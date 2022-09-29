
    ; NCP
    ; Timer 1 ISR with no C header
    
    .include "p33EP512GM604.inc"
    
    ; Variables used by the scheduler
    .extern task_stack_address
    .extern temp_W0
    .extern task_code_address_page
    .extern task_code_address_offset
    .extern new_task_stack_pointer
    .extern temp_stack_pointer
    .extern adr_current_task_stack_pointer
    ; ------------------------------------------
    
    .global _asmStackFrame
    
    .global _TSlock
    .global _TSunlock
    
    
    ; This function makes a TEST and SET operation on the file register 
    ; addressed by its first parameter (W0). Test and set operation is ever
    ; produced in bit 0 of the file register pointed by the first parameter.
    ; If this bit was 0 (non locked) the function
    ; replays with a 0 in its second parameter (W1), otherwise it replays with 
    ; a 1.
    ; The result of calling this function is set to 1 (locked in 
    ; any circunstance) the bit 0 of file register pointed by the first 
    ; parameter.
    
    ; ¡¡¡ATTENTION!!!
    ; TEST and SET operation is executed in only one instruction cycle.
    ; Therefore, it is uninterrumpible, hence it is impossible for race
    ; conditions to appear in relation to the bit in which TEST and SET 
    ; operates.
    
    ; Possible C prototipe
    ; void TSlock(int address_of_lock_variable, int address_of_answer);
    
_TSlock:
    PUSH    W2	    ; Needed inside the routine    
    BTSTS.Z [W0], #0
    BRA	    Z, itWasFree    ; Then the function answer is 0
    MOV	    #0x1, W2	    ; Otherwise the function answer is 1
    MOV	    W2, [W1]
    GOTO    endTSlock
itWasFree:
    MOV	    #0x0, W2
    MOV	    W2, [W1]
endTSlock:
    POP	    W2
    RETURN
    
   ; This function writes a 0 on the less significant bit of a file register
   ; used as a lock with a test a set instruction.
   ; This action should be interpreted as unlock a resource.
   ; The file register used as a lock is passed as a parameter (W0).
   
   ; Possible C prototipe
   ; void RCunlock(int address_of_lock_variable);
   
_TSunlock:
    PUSH    W1
    MOV	    [W0], W1
    AND	    W1, #0xE, [W0]
    POP	    W1
    RETURN
    
    
    ; Creates first stack frame of a task with current registers data
_asmStackFrame: 
    ; Save current stack pointer and W0 register
    MOV	    W15, _temp_stack_pointer
    MOV	    W0, _temp_W0
    
    ; Change stack to the task stack
    MOV	    _task_stack_address, W15
    
    ; Push task offset address of code
    MOV	    _task_code_address_offset, W0
    PUSH    W0
    
    ; Push initial SR<7:0> IPL3 PC<22:16> = tblpage (code page address)
    ; If another initial priority is needed then W0<15:13> must be used
    ; to contain initial IPL<2:0>
    MOV	    _task_code_address_page, W0
    AND.W   #0x007F, W0
    PUSH    W0
    
    ; Push registers
    MOV	    _temp_W0, W0    ; Recover W0
    
    PUSH    SR
    PUSH    W0
    PUSH    W1
    PUSH.D  W2
    PUSH.D  W4
    PUSH.D  W6
    PUSH.D  W8
    PUSH.D  W10
    PUSH.D  W12
    PUSH    W14
    PUSH    RCOUNT
    PUSH    TBLPAG
    PUSH    ACCAL
    PUSH    ACCAH
    PUSH    ACCAU
    PUSH    ACCBL
    PUSH    ACCBH
    PUSH    ACCBU
    PUSH    DCOUNT
    PUSH    DOSTARTL
    PUSH    DOSTARTH
    PUSH    DOENDL
    PUSH    DOENDH
    PUSH    CORCON
    PUSH    DSRPAG
    PUSH    DSWPAG
    
    ; Comunicate task stack pointer
    MOV	    W15, _new_task_stack_pointer
    
    ; Recover initial stack pointer
    MOV	    _temp_stack_pointer, W15
    
    return
    
      
    ; This saves context of current running task, then call scheduler and
    ; prepare context of the selected task to run
__T1Interrupt:
    ; Return addres is in stack + low byte of SR + IPL3
    
    BCLR    IFS0, #T1IF	    ; Clears the interrupt flag
    PUSH    SR
    PUSH    W0
    PUSH    W1
    PUSH.D  W2
    PUSH.D  W4
    PUSH.D  W6
    PUSH.D  W8
    PUSH.D  W10
    PUSH.D  W12
    PUSH    W14
    PUSH    RCOUNT
    PUSH    TBLPAG
    PUSH    ACCAL
    PUSH    ACCAH
    PUSH    ACCAU
    PUSH    ACCBL
    PUSH    ACCBH
    PUSH    ACCBU
    PUSH    DCOUNT
    PUSH    DOSTARTL
    PUSH    DOSTARTH
    PUSH    DOENDL
    PUSH    DOENDH
    PUSH    CORCON
    PUSH    DSRPAG
    PUSH    DSWPAG   
    
    ; Save old task stack pointer
    MOV	    _adr_current_task_stack_pointer, W0
    MOV	    W15, [W0]
    
    CALL    _count_tic
    CALL    _scheduler
    
    ; Charge new task stack pointer    
    MOV	    _adr_current_task_stack_pointer, W0
    MOV	    [w0], W15
    
    POP	    DSWPAG
    POP	    DSRPAG
    POP	    CORCON
    POP	    DOENDH
    POP	    DOENDL
    POP	    DOSTARTH
    POP	    DOSTARTL
    POP	    DCOUNT
    POP	    ACCBU
    POP	    ACCBH
    POP	    ACCBL
    POP	    ACCAU
    POP	    ACCAH
    POP	    ACCAL
    POP	    TBLPAG
    POP	    RCOUNT
    POP	    W14
    POP.D   W12
    POP.D   W10
    POP.D   W8
    POP.D   W6
    POP.D   W4
    POP.D   W2
    POP.D   W0
    POP	    SR

    RETFIE

    ; This saves context of current running task, then call scheduler and
    ; prepare context of the selected task to run
__INT2Interrupt:   
    ; Return addres is in stack + low byte of SR + IPL3
    
    BCLR    IFS1, #INT2IF	    ; Clears the interrupt flag
    PUSH    SR
    PUSH    W0
    PUSH    W1
    PUSH.D  W2
    PUSH.D  W4
    PUSH.D  W6
    PUSH.D  W8
    PUSH.D  W10
    PUSH.D  W12
    PUSH    W14
    PUSH    RCOUNT
    PUSH    TBLPAG
    PUSH    ACCAL
    PUSH    ACCAH
    PUSH    ACCAU
    PUSH    ACCBL
    PUSH    ACCBH
    PUSH    ACCBU
    PUSH    DCOUNT
    PUSH    DOSTARTL
    PUSH    DOSTARTH
    PUSH    DOENDL
    PUSH    DOENDH
    PUSH    CORCON
    PUSH    DSRPAG
    PUSH    DSWPAG
    
    ; Save old task stack pointer
    MOV	    _adr_current_task_stack_pointer, W0
    MOV	    W15, [W0]
    
    CALL    _scheduler
    
    ; Charge new task stack pointer    
    MOV	    _adr_current_task_stack_pointer, W0
    MOV	    [w0], W15
    
    POP	    DSWPAG
    POP	    DSRPAG
    POP	    CORCON
    POP	    DOENDH
    POP	    DOENDL
    POP	    DOSTARTH
    POP	    DOSTARTL
    POP	    DCOUNT
    POP	    ACCBU
    POP	    ACCBH
    POP	    ACCBL
    POP	    ACCAU
    POP	    ACCAH
    POP	    ACCAL
    POP	    TBLPAG
    POP	    RCOUNT
    POP	    W14
    POP.D   W12
    POP.D   W10
    POP.D   W8
    POP.D   W6
    POP.D   W4
    POP.D   W2
    POP.D   W0
    POP	    SR

    RETFIE
    
    
    .end

