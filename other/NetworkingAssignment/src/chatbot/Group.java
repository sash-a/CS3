/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package chatbot;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author actsha001
 */
public class Group {

    public int id;
    public List<Integer> clients;
    String groupName;

    public Group(int id, String [] users, String groupName) {
        this.id = id;
        clients = new ArrayList<>();
        for (String user : users) {
            clients.add(Integer.parseInt(user));
        }
        this.groupName = groupName;
    }
}
