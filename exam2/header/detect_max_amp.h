
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i


/* complete the state machine. */

void Detect_Max_Amp()
{
    switch(detect_max_amp_state)
    {
        case DMAInit:
		if (deq == 0) {
			detect_max_amp_state = DMAInit;
		}
		else {
			detect_max_amp_state = DMAeq;
		}
            break;
        case DMAeq:
		if (deq == 0x00) {
			detect_max_amp_state = DMAInit;
		}
		else {
			detect_max_amp_state = DMAeq;
		}
		break;
        default:
		detect_max_amp_state = DMAInit;
            break;
    }
    switch(detect_max_amp_state)
    {
        case DMAInit:
		maxamp = 0x00;
            break;
	case DMAeq:
		if (mamp > maxamp) {
			maxamp = mamp;
		}
        default:
		maxamp = 0x00;
            break;
    }
}
