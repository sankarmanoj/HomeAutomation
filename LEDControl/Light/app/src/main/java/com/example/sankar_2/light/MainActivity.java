package com.example.sankar_2.light;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Vibrator;
import android.preference.PreferenceManager;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;


import com.amazonaws.auth.BasicAWSCredentials;
import com.amazonaws.regions.Region;
import com.amazonaws.regions.Regions;
import com.amazonaws.services.sqs.AmazonSQS;
import com.amazonaws.services.sqs.AmazonSQSClient;
import com.amazonaws.services.sqs.model.SendMessageRequest;
import com.amazonaws.util.StringUtils;
import com.amazonaws.util.TimingInfo;

import java.io.IOException;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.sql.Time;


public class MainActivity extends Activity {


    private String serverIpAddress = "192.168.0.101";
    SeekBar bright;
    TextView value;
    Button reset;
    boolean nreg = true;
    AmazonSQS conn;
    BasicAWSCredentials cred;
    TextView ipaddr;
    DatagramSocket dsocket = null;
    DatagramPacket dp = null;
    String toSend;
    int count =0;
    Button bON,bOFF;
    boolean left = false;
    boolean ack = true;
    InetAddress serverAddr;
    String queueurl = "https://sqs.us-west-2.amazonaws.com/814828527445/droplet";
    boolean on=false,old_on= true;
    boolean connected;
    SharedPreferences.Editor editor;
    SharedPreferences prefs;

    SendMessageRequest turnon = new SendMessageRequest(queueurl,"value=255");
    SendMessageRequest turnoff = new SendMessageRequest(queueurl,"value=0");
    final int MAX_UDP_DATAGRAM_LEN = 1500;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        cred = new BasicAWSCredentials("","");
        conn = new AmazonSQSClient(cred);
        conn.setEndpoint("sqs.us-west-2.amazonaws.com");
        conn.setRegion(Region.getRegion(Regions.US_WEST_2));
        new Thread(new SQSsender(turnon)).start();

        setContentView(R.layout.activity_main);
        final Vibrator vib = (Vibrator)getApplicationContext().getSystemService(Context.VIBRATOR_SERVICE);
        bON = (Button)findViewById(R.id.TurnON);
        bON.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toSend = "value=" + String.valueOf(bright.getProgress());
                SendMessageRequest SQSmessage = new SendMessageRequest(queueurl,toSend);
                new Thread(new SQSsender(SQSmessage)).start();
                vib.vibrate(50);
            }
        });
        bOFF = (Button)findViewById(R.id.TurnOFF);
        bOFF.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toSend = "value=0";
                SendMessageRequest SQSmessage = new SendMessageRequest(queueurl,toSend);
                new Thread(new SQSsender(SQSmessage)).start();
                vib.vibrate(50);
            }
        });
        bright = (SeekBar) findViewById(R.id.seekBar);
        value = (TextView) findViewById(R.id.textView);

       ipaddr=(TextView)findViewById(R.id.IpAddr);
        reset = (Button)findViewById(R.id.Reset);
        prefs= PreferenceManager.getDefaultSharedPreferences(this);
        editor = prefs.edit();
        bright.setProgress(prefs.getInt("brightness",50));


        //ipaddr.setText("Server IP = "+serverIpAddress);
        reset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ack = true;
            }
        });
        Thread ct = new Thread(new ClientThread());
        ct.start();


        bright.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                toSend = "value="+String.valueOf(progress);
                editor.putInt("brightness",progress);
                editor.commit();
                Thread sending = new Thread(new Sender());
                sending.start();
            }
        });

    }
    public class SQSsender implements Runnable
    {
        SendMessageRequest SQStoSend;
        SQSsender(SendMessageRequest s)
        {
            SQStoSend = s;
        }
        @Override
        public void run()
        {
            conn.sendMessage(SQStoSend);
        }
    }
    public class ClientThread implements Runnable {
        @Override
        public void run() {
            try {

                dsocket = new DatagramSocket();
                serverAddr = InetAddress.getByName(serverIpAddress);
                new Receiver().execute(dsocket);

                Log.d("ClientActivity", "C: Closed.");
            } catch (Exception e) {
                Log.e("ClientActivity", "C: Error", e);
                connected = false;
            }

        }

    }

    public class Receiver extends AsyncTask<DatagramSocket,String,Void>
    {
        @Override
        protected Void doInBackground(DatagramSocket... params)
        {
            DatagramSocket sock = params[0];
            byte[] lmessage = new byte[MAX_UDP_DATAGRAM_LEN];
            DatagramPacket packet = new DatagramPacket(lmessage,lmessage.length);
            while (true){
                try {
                    sock.receive(packet);
                    String message = new String(lmessage,0,packet.getLength());
                    ack = true;
                    publishProgress(message);
                }
                catch(IOException e)
                {
                    e.printStackTrace();
                    break;
                }
            }
            return null;
        }
        @Override
        protected void onProgressUpdate(String... values)
        {
            super.onProgressUpdate();
            value.setText(values[0]);
            String gotval[] = values[0].split("=");
            int curval = Integer.parseInt(gotval[gotval.length-1]);
            bright.setProgress(curval);
        } 
    }
    public class Sender implements Runnable{
        @Override
        public void run()
        {
            dp = new DatagramPacket(toSend.getBytes(), toSend.length(), serverAddr, 12321);
            try {
                if(ack == true) {
                    dsocket.send(dp);
                    ack=false;
                }
            }
            catch(IOException e)
            {
                Log.e("Sending Message",e.getMessage());
            }
        }
    }





    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}