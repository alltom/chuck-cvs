//KiOm_object.ck 
//Adam R. Tindale

// see KiOm_objectTEST.ck for usage

public class KiOm{

	MidiIn min;
	MidiMsg msg;
	min.open(0);
	
	// stores the current val of xyz acceleromenter
	int xyz[4];
	0 => xyz[1] => xyz[2] => xyz[3];

	// other variables for the fancy stuff
	Event b1Timer;
	Event b2Timer;
	Event KiOm_x;
	Event KiOm_y;
	Event KiOm_z;
	Event KiOm_button1;
	Event KiOm_button2;
	Event KiOm_button1_dc;
	Event KiOm_button2_dc;

	dur b1dur;
	dur b2dur;

	300::ms => dur doubleclick;

	fun void port(int p){
		min.open(p);
	}
		
	fun void init(){
		spork ~ poller();
		spork ~ b1timer();
		spork ~ b2timer();
	}

	fun int x(){
		return xyz[1];
	}
	fun int y(){
		return xyz[2];
	}
	fun int z(){
		return xyz[3];
	}

	fun void doubleclickdur(dur dc){
		dc => doubleclick;
	}
	
	fun void b1timer(){

		dur lastclick;
		time beg;
		time end;
		while (true){
			
			//find the current time
			now => beg;
			
			// wait for the stop signal
			b1Timer => now;
			
			// get the current time after the signal
			now => end;
			
			// find the elapsed time and store it
			end - beg => lastclick;

			lastclick => b1dur;
		}
	}
	fun void b2timer(){

		dur lastclick;
		time beg;
		time end;
		while (true){
			now => beg;
			b2Timer=> now;
			now => end;
			end - beg => lastclick;
			lastclick => b2dur;
		}
	}	

	fun void poller(){
		1 => int mybool; 
		0 => int b1last;

		while(true){
			
			min => now;
			while( min.recv(msg) ) 
			{
				// buttons
				if (msg.data1 == 144)
				{
				          //button 1
					  if (msg.data2 == 1)
					{
						KiOm_button1.broadcast();
						
						b1Timer.signal();
						me.yield();

						if (b1dur < doubleclick && b1last == 1){
							<<<"doubleclick">>>;
							2 => b1last; 
						}
						else
						{	
							<<<"singleclick">>>;
							1 => b1last;
						}
						continue;
					}
					// button 2 freezes the current state
					if (msg.data2 == 2)
					{
						KiOm_button1.broadcast();

						b2Timer.signal();
						<<<"b2">>>;
					}

				}// end of buttons
				
				//accelerometers
				if (msg.data1 == 176){
					// stores vals in xyz
					msg.data3 => xyz[msg.data2];
					//throw some events
					if(msg.data2 ==1){
						KiOm_x.broadcast();	
						continue;
					}
					if(msg.data2 ==2){
						KiOm_y.broadcast();
						continue;
					}
					if(msg.data2 ==3){
						KiOm_z.broadcast();	
					}				
					
				}
				
			}
		}
	}
}