import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class TaxiSimulation
{

	public static void main(String[] args)
	{
		String file = args[0];

		try
		{
			Taxi taxi = readFileTaxi(file);
			List<Person> people = readFilePeople(file, taxi);
			for (Person p : people) p.start();
			taxi.start();
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}

	}

	private static Taxi readFileTaxi(String filename) throws Exception
	{
		BufferedReader f = new BufferedReader(new FileReader(filename));
		f.readLine(); // num people
		String line = f.readLine(); // num branches

		return new Taxi(Integer.parseInt(line));
	}

	private static List<Person> readFilePeople(String filename, Taxi taxi) throws
			IOException
	{
		List<Person> people = new ArrayList<>();
		BufferedReader f = new BufferedReader(new FileReader(filename));

		// Skipping
		f.readLine(); // num people
		f.readLine(); // num branches
		//

		String line = f.readLine();
		while(line != null)
		{
			List<Job> jobs = new ArrayList<>();
			int id = Integer.parseInt(line.split(" ")[0]);

			String[] splt = line.split(" \\(");
			for (int i = 1; i < splt.length; ++i)
			{
				String s = splt[i];
				String[] innerSplt = s.split(", ");

				// Special case for last job
				int endLen = 2;
				if (i == splt.length - 1) endLen = 1;

				jobs.add(new Job(Integer.parseInt(
						innerSplt[0]),
						Integer.parseInt(innerSplt[1].substring(0, innerSplt[1].length() - endLen))));
			}

			people.add(new Person(0, jobs.toArray(new Job[0]), id, taxi));

			line = f.readLine();
		}
		return people;
	}
}
