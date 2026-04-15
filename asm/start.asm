; Windows Research Starter - x64/x86 Assembly Harness
; Use IFDEF RAX to detect x64 environment

IFDEF RAX
    ; x64 specific setup
ELSE
    .model flat, c ; Required for x86 PROC usage
ENDIF

.code

; Example function: VoidFunc()
VoidFunc PROC
    ret
VoidFunc ENDP

END
