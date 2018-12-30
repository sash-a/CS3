package simulator;

import java.io.FileNotFoundException;
import java.io.IOException;

import java.util.ArrayDeque;
import java.util.Deque;
import simulator.ProcessControlBlock.State;

/**
 * Concrete Kernel type
 *
 * @author Stephan Jamieson
 * @version 8/3/15
 */
public class FCFSKernel implements Kernel
{

    private Deque<ProcessControlBlock> readyQueue;

    public FCFSKernel()
    {
        // Set up the ready queue.
        readyQueue = new ArrayDeque<>();
    }

    private ProcessControlBlock dispatch()
    {
        // Perform context switch, swapping process
        // currently on CPU with one at front of ready queue.
        // If ready queue empty then CPU goes idle ( holds a null value).
        // Returns process removed from CPU.
        // return null;

        CPU cpu = Config.getCPU();
//        if (cpu.getCurrentProcess() != null)
//        {
//            System.out.println(cpu.getCurrentProcess());
//        }
        /* 
         * If the queue is empty or the next instruction is an IO instruction
         * Then: make the cpu idle and add the current cpu program back to
         * the readyQueue if it still has instructions.
         */
        if (readyQueue.peek() == null || readyQueue.peek().getInstruction() instanceof IOInstruction)
        {
            ProcessControlBlock pcb = cpu.contextSwitch(null);

            if (pcb != null && pcb.hasNextInstruction())
            {
                readyQueue.add(pcb);
            }

            return pcb;
        }


        /*
         * else (queue not empty and next instruction is not IO):
         * context switch to the next instruction in the readyQueue and if the
         * current program has remaining instructions add to back to the readyQueue
         */
        ProcessControlBlock pcb = cpu.contextSwitch(readyQueue.pop());
        cpu.getCurrentProcess().setState(ProcessControlBlock.State.RUNNING);

        if (pcb != null && pcb.hasNextInstruction())
        {
            readyQueue.add(pcb);
        }

        return pcb;

    }

    public int syscall(int number, Object... varargs)
    {
        int result = 0;
        switch (number)
        {
            case MAKE_DEVICE:
            {
                IODevice device = new IODevice((Integer) varargs[0], (String) varargs[1]);
                Config.addDevice(device);
            }
            break;
            case EXECVE:
            {
                ProcessControlBlock pcb = this.loadProgram((String) varargs[0]);
                if (pcb != null)
                {
                    // Loaded successfully.
                    // Now add to end of ready queue.
                    readyQueue.add(pcb);
                    // If CPU idle then call dispatch.
                    if (Config.getCPU().isIdle())
                        dispatch();
                }
                else
                {
                    result = -1;
                }
            }
            break;
            case IO_REQUEST:
            {
                // IO request has come from process currently on the CPU.
                // Get PCB from CPU.
                // Find IODevice with given ID: Config.getDevice((Integer)varargs[0]);
                IODevice device = Config.getDevice((Integer) varargs[0]);
                // Make IO request on device providing burst time (varages[1]),
                // the PCB of the requesting process, and a reference to this kernel (so 
                // that the IODevice can call interrupt() when the request is completed.
                device.requestIO((Integer) varargs[1], Config.getCPU().getCurrentProcess(), this);
                // Set the PCB state of the requesting process to WAITING.
                Config.getCPU().getCurrentProcess().setState(ProcessControlBlock.State.WAITING);
                // Call dispatch().
                dispatch();
            }
            break;
            case TERMINATE_PROCESS:
            {
                // Process on the CPU has terminated.
                // Get PCB from CPU.
                // Set status to TERMINATED.
                Config.getCPU().getCurrentProcess().setState(ProcessControlBlock.State.TERMINATED);
                // Call dispatch().
                dispatch();
            }
            break;
            default:
                result = -1;
        }
        return result;
    }

    public void interrupt(int interruptType, Object... varargs)
    {
        switch (interruptType)
        {
            case TIME_OUT:
                throw new IllegalArgumentException("FCFSKernel:interrupt(" + interruptType + "...): this kernel does not suppor timeouts.");
            case WAKE_UP:
                // IODevice has finished an IO request for a process.
                // Retrieve the PCB of the process (varargs[1]), set its state
                // to READY, put it on the end of the ready queue.
                ProcessControlBlock pcb = ((ProcessControlBlock) varargs[1]);
                pcb.setState(ProcessControlBlock.State.READY);
                
                // If CPU is idle then dispatch().
                if (Config.getCPU().isIdle())
                    dispatch();
                break;
            default:
                throw new IllegalArgumentException("FCFSKernel:interrupt(" + interruptType + "...): unknown type.");
        }
    }

    private static ProcessControlBlock loadProgram(String filename)
    {
        try
        {
            return ProcessControlBlockImpl.loadProgram(filename);
        }
        catch (FileNotFoundException fileExp)
        {
            return null;
        }
        catch (IOException ioExp)
        {
            return null;
        }
    }
}
