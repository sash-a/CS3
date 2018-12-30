import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class Taxi extends Thread
{
	private int totalBranches;
	private int location;

	private int direction;

	private CopyOnWriteArrayList<Person> passengers;
	private CopyOnWriteArrayList<Person> hailed;

	private boolean wasIdle;

	public Taxi(int totalBranches)
	{
		passengers = new CopyOnWriteArrayList<>();
		hailed = new CopyOnWriteArrayList<>();

		location = 1;
		this.totalBranches = totalBranches;

		direction = 1;
		wasIdle = true;

		SimulatedTime.initial = SimulatedTime.getTime();
	}

	private boolean nextBranch()
	{
		// Idle if no one to pick up/drop off
		if (hailed.isEmpty() && passengers.isEmpty()) 
		{
			wasIdle = true;
			return false;
		}

		if (wasIdle)
		{
			// Didn't want to print this every time taxi moves so only print if taxi goes from idle to moving
			System.out.println(SimulatedTime.getClockTime() + " branch " + location + ": taxi depart");
			wasIdle = false;
		}

		int maxBranch = 0;
		int minBranch = totalBranches;

		// Checks if anyone hailed and is waiting 'further than taxi currently is'
		for (Person p : hailed)
		{
			if (p.location < minBranch) minBranch = p.location;

			if (p.location > maxBranch) maxBranch = p.location;
		}

		// Checks if passengers destination 'further than taxi currently is'
		for (Person p : passengers)
		{
			if (p.getNextJob().branchLocation < minBranch) minBranch = p.getNextJob().branchLocation;

			if (p.getNextJob().branchLocation > maxBranch) maxBranch = p.getNextJob().branchLocation;
		}

		// Turn around if past or at furthest point
		if (location <=  minBranch)
			direction = 1;
		else if(location >= maxBranch)
			direction = -1;

		location += direction;

		for (Person p : passengers) p.location = location;

		return true;
	}

	// Used by the taxi
	private boolean pickUp()
	{
		boolean pickedUp = false;
		Iterator<Person> it = hailed.iterator();
		List<Person> toRemove = new ArrayList<>();

		while(it.hasNext())
		{
			Person p = it.next();
			if (p.location != location) continue;

			// Allow people at the branch to get in the taxi i.e unfreeze the thread
			p.endWait();

			toRemove.add(p);
			pickedUp = true;
		}

		// Removing everyone who was picked up from the hailed list
		for (Person pers : toRemove)
			hailed.remove(pers);

		return pickedUp;
	}

	// Used by the taxi
	private boolean dropOff()
	{
		boolean droppedOff = false;
		Iterator<Person> it = passengers.iterator();
		List<Person> toRemove = new ArrayList<>();

		while(it.hasNext())
		{
			Person p = it.next();
			if (p.getNextJob() == null || p.getNextJob().branchLocation != location) continue;

			// Only people on taxi

			toRemove.add(p);
			// Allow people on the taxi to get off this is their destination i.e unfreeze the thread
			p.endWait();
			droppedOff = true;

		}

		// Removing everyone who was dropped off from the passenger list
		for (Person p : toRemove)
			passengers.remove(p);

		return droppedOff;
	}

	private void printArrived()
	{
		boolean printed = false;
		for (Person p : passengers)
		{
			if (p.getNextJob().branchLocation == location && !printed)
			{
				System.out.println(SimulatedTime.getClockTime() + " branch " + location + ": taxi arrive");
				printed = true;
			}
		}
		for (Person p : hailed)
		{
			if (p.location == location && !printed)
			{
				System.out.println(SimulatedTime.getClockTime() + " branch " + location + ": taxi arrive");
				printed = true;
			}
		}
	}

	// Called by the person
	void hail(Person p) { hailed.add(p); }

	// Called by the person
	void getIn(Person p) { passengers.add(p); }


	@Override
	public void run()
	{
		while(true)
		{
			try
			{
				printArrived();

				// Will wait 1 minute if pick ups or drop offs need to occur
				if (dropOff() | pickUp())
					SimulatedTime.wait(1);

				// Will wait 2 minutes if need to go to the next branch
				if (nextBranch())
					SimulatedTime.wait(2);
				else
					SimulatedTime.wait(1);
			}
			catch (InterruptedException e)
			{
				e.printStackTrace();
			}
		}
	}
}
