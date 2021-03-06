// Shane's Operating System Kernel
// kernel.c

void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt(int, char*, int, int);

void main() {

//	// ProjectA
//	int startVidMem = 0xb800;
//	int vidMemOffset = 0x0;
//	int white = 0x7;
//	char* letters = "Hello World\0";
//
//	while(*letters != 0x0) {
//		putInMemory(startVidMem, vidMemOffset, *letters);
//		vidMemOffset++;
//		putInMemory(startVidMem, vidMemOffset, white);
//		vidMemOffset++;
//		// advance letters pointer
//		letters++;
//	}

	// ProjectB
	//Step 1: Printing to the Screen - Interrupt 0x10
	//printString("Hello World");

	// Step 2: Reading from the keyboard - Interrupt 0x16
	//char line[80];
	//printString("Enter a line: ");
	//readString(line);
	//printString(line);

	// Step 3 - Read a sector from the disk - Interrupt 0x13
	//char buffer[512];
	//readSector(buffer, 30);
	//printString(buffer);

	// Step 4 - Create your own interrupt
	//makeInterrupt21();
	//interrupt(0x21, 0, 0, 0, 0);

	// Step 5 - Make printString, readString, and readSector interrupt calls
	char line[80];
	makeInterrupt21();
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);

	while(1);
}

// Step 1: Printing to the Screen - Interrupt 0x10
void printString(char* chars)
{
	int i=0;
	while(chars[i] != '\0') {
		//interrupt(0x10, 0xe*256+'Q', 0, 0, 0);
		interrupt(0x10, 0xe*256+chars[i], 0, 0, 0);
		i++;
	}
}

// Step 2: Reading from the keyboard - Interrupt 0x16
void readString(char* chars)
{
	int i=0;
	while(1) {
		if(i > 80) {
			return;
		}
		chars[i] = interrupt(0x16, 0, 0, 0, 0);
		if(chars[i] == 0xd) {
			interrupt(0x10, 0xe*256+0xd, 0, 0, 0);
			interrupt(0x10, 0xe*256+0xa, 0, 0, 0);
			chars[i+1] = 0xa;
			chars[i+2] = 0x0;
			return;
		}
		else if(chars[i] == 0x8) {
			if(i > 0) {
				interrupt(0x10, 0xe*256+chars[i], 0, 0, 0);
				i--;
				chars[i] = ' ';
				interrupt(0x10, 0xe*256+chars[i], 0, 0, 0);
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
			}
		}
		else if(chars[i] != ' ') {
			interrupt(0x10, 0xe*256+chars[i], 0, 0, 0);
			i++;
		}
	}
}

// Step 3 - Read a sector from the disk - Interrupt 0x13
void readSector(char* buffer, int sector)
{
	int relativeSector;
	int head;
	int track;
	relativeSector = mod(sector, 18) + 1;
	head = mod(div(sector, 18), 2);
	track = div(sector, 36);
	interrupt(0x13, 0x2*256+0x1, buffer, track*256+relativeSector, head*256+0);
}

int mod(int a, int b)
{
	while(a>=b) {
		a=a-b;
	}
	return a;
}

int div(int a, int b)
{
	int c=0;
	while((c+1)*b<=a) {
		c++;
	}
	return c;
}

// Step 4 - Create your own interrupt & Step 5 - Make printString, readString, and readSector interrupt calls
void handleInterrupt21(int ax, char* bx, int cx, int dx)
{
	if(ax=0) {
		printString(bx);
	}
	if(ax=1) {
                readString(bx);
        }
	if(ax=2) {
                readSector(bx, cx);
        }
	else {
		printString("error message");
	}
}
