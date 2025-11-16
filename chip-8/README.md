### ROMs

|ROM|Reference|
|---|---|
|[Helloworld](test/helloworld)|[mwales repo](https://github.com/mwales/chip8/blob/master/customRom/helloworld.md)|


### Tests


#### `CLS`

Clears the display // Clears the screen

Test Snippet :

```
0000000 e000
0000002
```

#### `LD Vx, Byte`

Loads the immediate value into the register

Test Snippet :

```
0000000 1062 1162
0000004
```





### References

- [Chip8 Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [Chip8 by HPMANIAC](https://web.archive.org/web/20140825173007/http://vanbeveren.byethost13.com/stuff/CHIP8.pdf)
- [mwales's chip8 emulator](https://github.com/mwales/chip8/tree/master)
