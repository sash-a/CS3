public class Job
{

	public int branchLocation;
	public int jobDuration;
	public long arrivalTime;

	public Job(int branchLoc, int timeToWork)
	{
		this.branchLocation = branchLoc;
		this.jobDuration = timeToWork;

		arrivalTime = -1;
	}

	boolean isFinished()
	{
		return System.currentTimeMillis() > (arrivalTime + jobDuration * 33);
	}
}
