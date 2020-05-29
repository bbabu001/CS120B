
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i

static int i = 0;

/*complete the state machine*/

void Ping()
{
    switch(ping_state)
    {
        case PInit:
	    ping_state = POn;
	    i = 0;
            break;
	case POn:
	    if (i < 1) {
		    ping_state = POn;
		    i++;
	    }
	    else {
		    ping_state = POff;
		    i = 0;
	    }
	    break;
	case POff:
	    if (i < 9) {
		    ping_state = POff;
		    i++;
	    }
	    else {
		    ping_state = POn;
		    i = 0;
	    }
	    break;
        default:
            ping_state = PInit;
	    i = 0;
            break;
    }
    switch(ping_state)
    {
        case PInit:
            break;
	case POn:
	    Pout = 0x01;
	    break;
	case POff:
	    Pout = 0x00;
	    break;
        default:
            break;
    }
}
