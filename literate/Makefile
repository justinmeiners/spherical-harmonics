SRC=sh.lit
DEMO_DIR=../demo/

all: tangle html

tangle:
	lit --tangle --out-dir ${DEMO_DIR} ${SRC}

html:
	lit --weave ${SRC}

