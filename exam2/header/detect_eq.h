//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i

static int j;

/*complete the state machine*/

void Detect_EQ()
{
    switch(detect_eq_state)
    {
        case DEQInit:
            mamp = PINA & 0xF8;
	    j = 0;
	    if (mamp == 0) {
		detect_eq_state = DEQInit;
		}
	    else {
		detect_eq_state = detect;
		}
            break;
	case detect:
		detect_eq_state = dwait;
		break;
	case dwait:
		if (j < 10 && mamp == 0) {
			j++;
		}
		else if (j >= 10 && mamp == 0) {
			detect_eq_state = DEQInit;
			j = 0;
		}
		else {
			j = 0;
			detect_eq_state = dwait;
		}
		break;
        default:
            detect_eq_state = DEQInit;
	    j = 0;
            break;
    }
    switch(detect_eq_state)
    {
        case DEQInit:
	    deq = 0x00;
            break;
	case detect:
		deq = 0x02;
		break;
	case dwait:
		break;
        default:
	    deq = 0x00;
            break;
    }
}
