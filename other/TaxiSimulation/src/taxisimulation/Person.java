import java.util.concurrent.Semaphore;

public class Person extends Thread
{

	private int id;
	int location;

	private Job[] jobs;
	private int current;

	// TODO make private
	private Semaphore seat; // refactor to seat

	private Taxi taxi;

	Person(int loc, Job[] jobs, int id, Taxi t)
	{
		this.id = id;


		taxi = t;

		current = -1;
		this.jobs = jobs;
		this.jobs[0].arrivalTime = 0;

		location = loc;

		seat = new Semaphore(0);
	}

	private synchronized boolean hail() throws InterruptedException
	{
		if (current == jobs.length - 1 && jobs[current].isFinished()) return false; // should terminate

		System.out.println(SimulatedTime.getClockTime() + " branch " + location + ": person " + id + " hail");

		taxi.hail(this);
		seat.acquire();
		return true;
	}

	private void nextJob(long time)
	{
		if (!hasNextJob()) return;

		current++;
		jobs[current].arrivalTime = time;
	}

	private boolean hasNextJob() { return current < jobs.length - 1; }

	Job getNextJob()
	{
		if (!hasNextJob())
			return null;

		return jobs[current + 1];
	}

	private void getIn() throws InterruptedException
	{
		System.out.println(SimulatedTime.getClockTime() + " branch " + location + ": person " + id + " request " +
				getNextJob().branchLocation);

		taxi.getIn(this);
		seat.acquire();
	}

	void endWait() { seat.release(); }

	@Override
	public void run()
	{
		while(current < jobs.length - 1 || !jobs[current].isFinished())
		{
			try
			{
				if (current == -1 || jobs[current].isFinished()) // Only hail the taxi if finished job
				{
					if (!hail()) continue;
					getIn();
					System.out.println(SimulatedTime.getClockTime() + " branch " + location + ": person " + id + " " +
							"disembark");
					nextJob(SimulatedTime.getTime());
				}
			}
			catch (InterruptedException e)
			{
				e.printStackTrace();
			}
		}
	}
}
