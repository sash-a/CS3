/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package simulator;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author abrsas002
 */
public class ProcessControlBlockImpl implements ProcessControlBlock
{

    static int pid = 0;

    int progID;
    int priority;
    String name;
    State state;
    int pc;
    List<Instruction> instructions;

    public ProcessControlBlockImpl(String name)
    {
        ProcessControlBlockImpl.pid++;

        instructions = new ArrayList<>();
        state = state.READY;
        priority = 0;
        pc = 0;

        this.name = name;
        progID = pid;
    }

    public static ProcessControlBlock loadProgram(String file) throws IOException
    {
        char HASH = '#';

        ProcessControlBlockImpl pcb = new ProcessControlBlockImpl(file);
        BufferedReader f = new BufferedReader(new FileReader(file));

        String line = f.readLine();

        while (line != null)
        {
            char first = line.toCharArray()[0];
            if (first == HASH)
            {
                line = f.readLine();
                continue;
            }

            Instruction instruction = null;
            String[] splitLine = line.split(" ");

            if (splitLine[0].equals("CPU"))
            {
                instruction = new CPUInstruction(Integer.parseInt(splitLine[1]));
            }
            else if (splitLine[0].equals("IO"))
            {
                instruction = new IOInstruction(
                        Integer.parseInt(splitLine[1]),
                        Integer.parseInt(splitLine[2]));
            }

            pcb.instructions.add(instruction);

            line = f.readLine();
        }

        return pcb;
    }

    @Override
    public int getPID()
    {
        return progID;
    }

    @Override
    public String getProgramName()
    {
        return name;
    }

    @Override
    public int getPriority()
    {
        return priority;
    }

    @Override
    public int setPriority(int value)
    {
        int temp = priority;
        priority = value;
        return temp;
    }

    @Override
    public Instruction getInstruction()
    {
        return instructions.get(pc);
    }

    @Override
    public boolean hasNextInstruction()
    {
        return pc != instructions.size() - 1;
    }

    @Override
    public void nextInstruction()
    {
        pc++;
    }

    @Override
    public State getState()
    {
        return state;
    }

    @Override
    public void setState(State state)
    {
        this.state = state;
    }

    //< Time: 0000000001 Kernel: Context Switch {Idle}, process(pid=1, state=READY, name="pOne.prg")).
    @Override
    public String toString()
    {
        return "process(pid=" + progID + ", state=" + state + ", name=\"" + name + "\")";
    }
}
