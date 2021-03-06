/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gui_design;

import chatbot.Client.Client;
import chatbot.Message;

import java.awt.event.KeyEvent;
import java.io.File;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

/**
 * @author actsha001
 */
public class ChatRoom extends javax.swing.JFrame
{

	public Client user;
	public int otherID;
	public boolean isGroup;
	ChatRoom This;

	public ChatRoom(Client client, int otherID, boolean isGroup)
	{
		initComponents();
		This = this;

		user = client;
		this.otherID = otherID;
		this.isGroup = isGroup;
		if (!isGroup)
		{
			client.addChatRoom(this);
		}
		else
		{
			btnFile.setEnabled(false); // so that you cannot send files to a group.
		}

		txtMessage.requestFocus();

		this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		this.addWindowListener(new java.awt.event.WindowAdapter()
		{
			@Override
			public void windowClosing(java.awt.event.WindowEvent windowEvent)
			{
				if (!isGroup)
				{
					user.privateChatRooms.remove(otherID);//informs client it no longer has a chatroom for this person
				}
				This.dispose();
			}
		});
	}

	public void recieveMessage(String message)
	{
		String otherName = user.gui.knownClients.get(otherID);
		int key = user.id * 500 + (otherID + 1);
		String m = Message.decryptMessage(message, key);
		txaChat.append(otherName + ": " + m + "\n");
	}

	public void groupRecieveMessage(String message, int senderID)
	{
		String senderName = user.gui.knownClients.get(senderID);
		String m = Message.decryptMessage(message, senderID + 1 + otherID * 5000);
		txaChat.append(senderName + ": " + m + "\n");
	}

	private void sendMessage()
	{
		if (txtMessage.getText().equals(""))
		{
			return;
		}

		txaChat.append(user.name + ": " + txtMessage.getText() + "\n");

		int key = (user.id + 1) + (isGroup ? otherID * 5000 : otherID * 500);

		String em = Message.encryptMessage(txtMessage.getText(), key);
		if (isGroup)
		{
			user.sendToServer("gmessage:" + user.id + ":" + otherID, em);
		}
		else
		{
			user.sendToServer("message:" + user.id + ":" + otherID, em);
		}
		txtMessage.setText("");
	}

	public void closeChatRoom()
	{
		this.dispose();
	}

	/**
	 * This method is called from within the constructor to initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is always
	 * regenerated by the Form Editor.
	 */
	@SuppressWarnings("unchecked")
	// <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
	private void initComponents()
	{

		jScrollPane1 = new javax.swing.JScrollPane();
		txaChat = new javax.swing.JTextArea();
		txtMessage = new javax.swing.JTextField();
		btnSend = new javax.swing.JButton();
		btnFile = new javax.swing.JButton();

		setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

		txaChat.setEditable(false);
		txaChat.setColumns(20);
		txaChat.setRows(5);
		jScrollPane1.setViewportView(txaChat);

		txtMessage.addKeyListener(new java.awt.event.KeyAdapter()
		{
			public void keyPressed(java.awt.event.KeyEvent evt)
			{
				txtMessageKeyPressed(evt);
			}
		});

		btnSend.setText("Send");
		btnSend.addActionListener(new java.awt.event.ActionListener()
		{
			public void actionPerformed(java.awt.event.ActionEvent evt)
			{
				btnSendActionPerformed(evt);
			}
		});

		btnFile.setText("Send file");
		btnFile.addActionListener(new java.awt.event.ActionListener()
		{
			public void actionPerformed(java.awt.event.ActionEvent evt)
			{
				btnFileActionPerformed(evt);
			}
		});

		javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
		getContentPane().setLayout(layout);
		layout.setHorizontalGroup(
				layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
						.addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
								.addContainerGap()
								.addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
										.addComponent(jScrollPane1)
										.addComponent(txtMessage, javax.swing.GroupLayout.DEFAULT_SIZE, 384, Short
												.MAX_VALUE))
								.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
								.addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
										.addComponent(btnSend, javax.swing.GroupLayout.PREFERRED_SIZE, 73, javax.swing
												.GroupLayout.PREFERRED_SIZE)
										.addComponent(btnFile))
								.addContainerGap())
		);
		layout.setVerticalGroup(
				layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
						.addGroup(layout.createSequentialGroup()
								.addContainerGap()
								.addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
										.addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 258, Short
												.MAX_VALUE)
										.addGroup(layout.createSequentialGroup()
												.addGap(0, 0, Short.MAX_VALUE)
												.addComponent(btnFile, javax.swing.GroupLayout.PREFERRED_SIZE, 59,
														javax.swing.GroupLayout.PREFERRED_SIZE)))
								.addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
								.addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
										.addComponent(txtMessage)
										.addComponent(btnSend, javax.swing.GroupLayout.DEFAULT_SIZE, 34, Short
												.MAX_VALUE))
								.addGap(7, 7, 7))
		);

		pack();
	}// </editor-fold>//GEN-END:initComponents

	private void btnSendActionPerformed(java.awt.event.ActionEvent evt)
	{//GEN-FIRST:event_btnSendActionPerformed
		sendMessage();
	}//GEN-LAST:event_btnSendActionPerformed

	private void txtMessageKeyPressed(java.awt.event.KeyEvent evt)
	{//GEN-FIRST:event_txtMessageKeyPressed
		if (evt.getKeyCode() == KeyEvent.VK_ENTER)
		{
			sendMessage();
		}
	}//GEN-LAST:event_txtMessageKeyPressed

	private void btnFileActionPerformed(java.awt.event.ActionEvent evt)//GEN-FIRST:event_btnFileActionPerformed
	{//GEN-HEADEREND:event_btnFileActionPerformed
		JFileChooser chooser = new JFileChooser();
		chooser.setCurrentDirectory(new java.io.File("."));
		chooser.setDialogTitle("Send file");
		chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		chooser.setAcceptAllFileFilterUsed(false);

		if (chooser.showOpenDialog(null) == JFileChooser.APPROVE_OPTION)
		{
			File file = chooser.getSelectedFile();
			user.sendFileToServer(file, otherID);
		}
	}//GEN-LAST:event_btnFileActionPerformed


	// Variables declaration - do not modify//GEN-BEGIN:variables
	private javax.swing.JButton btnFile;
	private javax.swing.JButton btnSend;
	private javax.swing.JScrollPane jScrollPane1;
	private javax.swing.JTextArea txaChat;
	private javax.swing.JTextField txtMessage;
	// End of variables declaration//GEN-END:variables
}
