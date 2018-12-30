
import java.util.Scanner;
import simulator.FCFSKernel;
import simulator.Config;
import simulator.Kernel;
import simulator.SystemTimer;
import simulator.TRACE;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author abrsas002
 */
public class SimulateFCFS
{
    public static void main(String[] args)
    {
        // Getting input
        Scanner sc = new Scanner(System.in);

        System.out.println("*** FCFS Simulator ***");
        System.out.print("Enter configuration file name: ");
        String configFile = sc.nextLine();

        System.out.print("Enter cost of system call: ");
        int sysCallCost = Integer.parseInt(sc.nextLine());

        System.out.print("Enter cost of context switch: ");
        int contextSwitchCost = Integer.parseInt(sc.nextLine());

        System.out.print("Enter trace level: ");
        int traceLevel = Integer.parseInt(sc.nextLine());

        // Running simulation
        TRACE.SET_TRACE_LEVEL(traceLevel);
        final Kernel kernel = new FCFSKernel();

        Config.init(kernel, contextSwitchCost, sysCallCost);
        Config.buildConfiguration(configFile);
        Config.run();

        // Printing results
        SystemTimer timer = Config.getSystemTimer();
        System.out.println(timer);
        System.out.println(
                "Context switches: "
                + Config.getCPU().getContextSwitches()
        );
        System.out.printf(
                "CPU utilization: %.2f\n",
                ((double) timer.getUserTime()) / timer.getSystemTime() * 100);
    }
}
