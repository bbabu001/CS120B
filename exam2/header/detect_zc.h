
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i

static char pstate;

/*complete the state machine*/

void Detect_ZC()
{
	mdir = PINA & 0x07;
    switch(detect_zc_state)
    {
        case DZCInit:		
		if (mdir == 0x00) {
			detect_zc_state = E;
			pstate = 1;
		}
		else if (mdir == 0x01) {
			detect_zc_state = NE;
			pstate = 2;
		}
		else if (mdir == 0x02) {
			detect_zc_state = N;
			pstate = 3;
		}
		else if (mdir == 0x03) {
			detect_zc_state = NW;
			pstate = 4;
		}
		else if (mdir == 0x04) {
			detect_zc_state = W;
			pstate = 5;
		}
		else if (mdir == 0x05) {
			detect_zc_state = sW;
			pstate = 6;
		}
		else if (mdir == 0x06) {
			detect_zc_state = S;
			pstate = 7;
		}
		else {
			detect_zc_state = sE;
			pstate = 8;
		}
            break;
        case E:
		if (mdir == 0x04 && pstate == 1) {
			detect_zc_state = W;
			pstate = 5;
			zc = 0x04;
		}
		else {
			detect_zc_state = E;
			pstate = 1;
			zc = 0x00;
		}
		break;
	case NE:
		if (mdir == 0x05 && pstate == 2) {
			detect_zc_state = sW;
			pstate = 6;
			zc = 0x04;
		}
		else {
			detect_zc_state = NE;
			pstate = 2;
			zc = 0x00;
		}
		break;
	case N:
		if (mdir == 0x06 && pstate == 3) {
			detect_zc_state = S;
			pstate = 7;
			zc = 0x04;
		}
		else {
			detect_zc_state = N;
			pstate = 3;
			zc = 0x00;
		}
		break;
	case NW:
		if (mdir == 0x07 && pstate == 4) {
			detect_zc_state = sE;
			pstate = 8;
			zc = 0x04;
		}
		else {
			detect_zc_state = NW;
			pstate = 4;
			zc = 0x00;
		}
		break;
	case W:
		if (mdir == 0x00 && pstate == 5) {
			detect_zc_state = E;
			pstate = 1;
			zc = 0x04;
		}
		else {
			detect_zc_state = W;
			pstate = 5;
			zc = 0x00;
		}
		break;
	case sW:
		if (mdir == 0x01 && pstate == 6) {
			detect_zc_state = NE;
			pstate = 2;
			zc = 0x04;
		}
		else {
			detect_zc_state = sW;
			pstate = 6;
			zc = 0x00;
		}
		break;
	case S:
		if (mdir == 0x02 && pstate == 7) {
			detect_zc_state = N;
			pstate = 3;
			zc = 0x04;
		}
		else {
			detect_zc_state = S;
			pstate = 7;
			zc = 0x00;
		}
		break;
	case sE:
		if (mdir == 0x03 && pstate == 8) {
			detect_zc_state = NW;
			pstate = 4;
			zc = 0x04;
		}
		else {
			detect_zc_state = sE;
			pstate = 8;
			zc = 0x00;
		}
		break;
        default:
            detect_zc_state = DZCInit;
		zc = 0x00;
		pstate = 0;
            break;
    }
    switch(detect_zc_state)
    {
        case DZCInit:
            break;
        default:
            break;
    }
}
