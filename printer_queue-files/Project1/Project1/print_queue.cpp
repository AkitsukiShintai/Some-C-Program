#include "print_queue.h"

////////////////////////////////////////////////////////////
/* PrintQueue implementation */
////////////////////////////////////////////////////////////

/* constructor 
 * pointer to a simulator that's running it
 */ 
PrintQueue::PrintQueue(Simulator* _psim) : 
	psim(_psim),num_available_printers(0) , printer_status(), jobs_in_queue()
{}

/* to use a printer we need to register it with the print queue first */
void PrintQueue::RegisterPrinter(int ID, double speed) {
	/* pair of (ID and a pair (available, speed) ) */
//	printer_status.insert( 
//			std::make_pair<int,std::pair<bool,double> >
//			         (ID,std::make_pair<bool,double>(true,speed) ) 
//			);
//next line is equivalent to the above insert
	printer_status[ ID ] = std::pair<bool,double>(true,speed);
	//printer is available by default
	++num_available_printers;
}

/* implements 2) from PrintQueue */
void PrintQueue::JobFinished( double current_time, int printerID, PrintJob job)
{
	ID2pair::iterator it = printer_status.find( printerID );

	if ( it == printer_status.end() ) throw "invalid printerID";

	std::cout << "Printer " << printerID << " finished job " 
		<< job.jobID << " at time " << current_time << std::endl;

	if ( !jobs_in_queue.empty() ) { //immediately assign another job
		//...
		std::pair<PrintJob,double> newjob = jobs_in_queue.top();
		std::cout << "Printer " << printerID << " assigned new job " 
			<< newjob.first.jobID<< " at time " << current_time << std::endl;
		psim->AddEvent(new  EventJobFinished(newjob.first.size / (*it).second.second + current_time, newjob.first, (*it).first, this, &PrintQueue::JobFinished));
		//--num_available_printers;
		jobs_in_queue.pop();
	} else { // no jobs in the queue - printer rests
		++num_available_printers;
		it->second.first = true;
	}
}

/* implements 3) from PrintQueue */
void PrintQueue::NewJobArrived( double current_time, PrintJob job )
{
	std::cout << "New job " << job.jobID 
		<< " at time " << current_time << std::endl;

	if ( num_available_printers>0 ) { //self-explanatory
		//find an available printer
		ID2pair::iterator it = printer_status.begin();
		while ( it != printer_status.end() && it->second.first==false ) {++it;}

		//if condition holds, we have a problem with data: 
		//num_available_printers>0 but no printer seem to be available
		if (it==printer_status.end()) throw "corrupted data";

		//...

		std::cout << "Job assigned to printer " << it->first
			<< " at time " << current_time << std::endl;
		psim->AddEvent(new  EventJobFinished(job.size / (*it).second.second + current_time, job, (*it).first,this, &PrintQueue::JobFinished));
		--num_available_printers;
		it->second.first = false;
	} else { 
		std::cout << "No available printers - put in print queue\n";
		//...
		jobs_in_queue.push(std::make_pair( job,current_time));
	}


}

////////////////////////////////////////////////////////////
/* EventJobFinished implementation */
////////////////////////////////////////////////////////////
EventJobFinished::EventJobFinished(
	double when,
	const PrintJob& _job,
	const int _printerID,
	PrintQueue* _p_print_queue,
	void (PrintQueue::*_p_method)(double, int, PrintJob)
) :job(_job), p_print_queue(_p_print_queue), printerID(_printerID),p_method(_p_method) {

	SetWhen(when);
}

void EventJobFinished::Execute() {
	(p_print_queue->*p_method)(When(), printerID, job);
}

////////////////////////////////////////////////////////////
/* EventNewJobArrived implementation */
////////////////////////////////////////////////////////////
EventNewJobArrived::EventNewJobArrived(
	double when,
	const PrintJob& _job,
	PrintQueue* _p_print_queue,
	void (PrintQueue::*_p_method)(double, PrintJob)
):job(_job), p_print_queue(_p_print_queue), p_method(_p_method){

	SetWhen(when);
}

void EventNewJobArrived::Execute() {
	(p_print_queue->*p_method)(When(),job);
}