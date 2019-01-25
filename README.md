# cachesim
Cache memoriaren simuladorea

# Konpilazioa eta beharrezko liburutegiak

Simuladorea exekutatzeko beharrezko liburutegiak C-ko estandarrak dira.
Konpilatzeko, 'make' idaztea nahikoa da eta automatikoki sortuko du
zure sistemak behar duen 'sim' izeneko exekutagarria.
Hala ere, 32 edo 64 biteko exekutagarria sortu nahi baldin bada, posible da
agindu hauekin:
32bit: 'make sim32'
64bit: 'make sim64'

# Erabilera

Simuladoreak aginduen bidez funtzionatzen du. Aginduen zerrenda idazteko,
'help' idaztea nahikoa da. Hauek dira aginduak:

reset					simuladorea hasieratu
rd <helbidea>				helbideko datua kargatu
wr <helbidea>				helbidean datua gorde
status					cachearen egoera ikusi
word <tamaina>				hitzaren tamaina definitu (byte)
block <tamaina>				blokeen tamaina (byte) definitu
set <kopurua>				multzoen tamaina (1,2,4,8) definitu
replacement <fifo, lru>			ordezkapen-politika definitu
help					laguntza hau erakutsi
exit					simulatzailetik irten

# Agindu-fitxategiaren formatua

Agindu fitxategiak ondorengo formatua izan behar du eta lerro bakoitzean
agindu soil bat egon behar da:
rd/wr @helbidea
Adibidez:
wr 5000
ld 1600

# Agindu-fitxategiaren irteera

Eragiketa guztien emaitzak 'exec_output' izeneko fitxategian gordetzen dira.
