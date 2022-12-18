`?` = Wildcard nibble (4 bits)

# Obfuscated Push

## Varient 1
`push REGISTER`
```
mov    QWORD PTR [rsp-0x8], REGISTER
lea    rsp, [rsp-0x8]
```

Example, `push rax`:
```
48 89 44 24 F8    ; mov    QWORD PTR [rsp-0x8], rax
48 8D 64 24 F8    ; lea    rsp, [rsp-0x8]
```

## Varient 2

```
lea    rsp, [rsp-0x8]
mov    QWORD PTR [rsp], REGISTER
```

Example, `push rax`:
```
48 8D 64 24 F8    ; lea    rsp, [rsp-0x8]
48 89 04 24       ; mov    QWORD PTR [rsp], rax
```

# Obfuscated Pop

## Varient 1

`pop REGISTER`
```
lea    rsp, [rsp+0x8]
mov    REGISTER, QWORD PTR [rsp-0x8]
```

Example, `pop rax`:
```
48 8D 64 24 08    ; lea    rsp, [rsp+0x8]
48 8B 44 24 F8    ; mov    rax, QWORD PTR [rsp-0x8]
```

## Varient 2

`pop REGISTER`
```
mov    REGISTER, QWORD PTR [rsp]
lea    rsp, [rsp+0x8]
```

Example, `pop rax`:
```
48 8B 04 24       ; mov    rax, QWORD PTR [rsp]
48 8D 64 24 08    ; lea    rsp, [rsp+0x8]
```

# Obfuscated Return
```
48 8D 64 24 08    ; lea    rsp, [rsp+0x8]
FF 64 24 F8       ; jmp    QWORD PTR [rsp-0x8]
```

Simplifies to:
```
C3    ; ret
```

# Obfuscated Jump

```
55                   ; push    rbp
48 8D 2D ?? ?? ?? ?? ; lea     rbp, [rip+0xOFFSET] ; jmp_location
48 87 2C 24          ; xchg    rbp, [rsp]
C3                   ; ret
```

Elements of this obfuscated jump can be obfuscated further.
`push rbp` can be transformed into an obfuscated varient.
`ret` can be transformed into an obfuscated varient. 

However they all simplify to:
```
E9 ?? ?? ?? ??    ; jmp     location
```

# Obfuscated Conditional Jump

```
55                            ; push    rbp
48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ; mov     *, offset UNCONDITIONAL_LOCATION
48 87 2C ??                   ; xchg    *, *
5?                            ; push    *
5?                            ; push    *
48 8B ?? 24 10                ; mov     *, [rsp+10h]
48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ; mov     *, offset CONDITIONAL_LOCATION
4? 0F ?? ??                   ; cmov*   *, *
48 89 ?? 24 10                ; mov     [rsp+10h], *
5?                            ; pop     *
5?                            ; pop     *
C3                            ; ret
```

Simplifies to:

```
0F ?? ?? ?? ?? ??    ; j*   CONDITIONAL_LOCATION
E9 ?? ?? ?? ??       ; jmp  UNCONDITIONAL_LOCATION
```

For example, if `cmov*` in the obfuscated code was `cmove`, then the simplified code would be:

```
0F 84 ?? ?? ?? ??    ; je   CONDITIONAL_LOCATION
E9 ?? ?? ?? ??       ; jmp  UNCONDITIONAL_LOCATION
```
