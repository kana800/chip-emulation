STR,	FIM	0P 224
	SRC	OP
	LDM	1	/ Select ROM port 14
	WRR		/ Turn on write enable
	JMS	COM	/ Routine COM setup PRAM
			/ address
