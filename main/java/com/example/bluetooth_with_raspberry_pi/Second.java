package com.example.bluetooth_with_raspberry_pi;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

public class Second extends AppCompatActivity {
    BluetoothAdapter mBluetoothAdapter;
    Set<BluetoothDevice> mPairedDevices;
    List<String> mListPairedDevices;

    BluetoothDevice mBluetoothDevice;
    BluetoothSocket mBluetoothSocket;
    ConnectedBluetoothThread mThreadConnectedBluetooth;
    Handler mBluetoothHandler;

    final static int BT_REQUEST_ENABLE = 1;
    final static int BT_MESSAGE_READ = 2;
    final static int BT_CONNECTING_STATUS = 3;
    final static UUID BT_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    int connNum = 0;

    Toolbar tb;

    ImageButton bluetooth_on;
    ImageButton bluetooth_off;
    ImageButton connection_on;

    TextView bluetooth_state;
    TextView connection_state;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_second);

        bluetooth_on = findViewById(R.id.btn_bluetooth_on);
        bluetooth_off = findViewById(R.id.btn_bluetooth_off);
        connection_on = findViewById(R.id.btn_connection);

        bluetooth_state = findViewById(R.id.tv_bluetooth_st);
        connection_state = findViewById(R.id.tv_connection_st);

        bluetooth_on.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bluetoothOn();
            }
        });

        bluetooth_off.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bluetoothOff();
            }
        });

        connection_on.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                listPairedDevices();
            }
        });

    }

    void bluetoothOn(){
        if(mBluetoothAdapter == null){
            Toast.makeText(getApplicationContext(), "블루투스를 지원하지 않는 기기입니다.", Toast.LENGTH_SHORT).show();
        }
        else {
            if(mBluetoothAdapter.isEnabled()){
                Toast.makeText(getApplicationContext(), "블루투스가 이미 활성화 되어있습니다.", Toast.LENGTH_SHORT).show();
                bluetooth_state.setText("블루투스 ON");
            }
            else{
                Toast.makeText(getApplicationContext(), "블루투스가 활성화 되어 있지 않습니다.", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(intent, BT_REQUEST_ENABLE);
            }
        }
    }// bluetooth On End

    void bluetoothOff() {
        if (mBluetoothAdapter.isEnabled()) {
            mBluetoothAdapter.disable();
            Toast.makeText(getApplicationContext(), "블루투스가 비활성화 되었습니다.", Toast.LENGTH_SHORT).show();
            bluetooth_state.setText("비활성화");
        }
        else {
            Toast.makeText(getApplicationContext(), "블루투스가 이미 비활성화 되어 있습니다.", Toast.LENGTH_SHORT).show();
        }
    }// bluetooth Off End

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        switch (requestCode){
            case BT_REQUEST_ENABLE:
                if(resultCode == RESULT_OK){
                    Toast.makeText(getApplicationContext(), "블루투스 활성화", Toast.LENGTH_SHORT).show();
                    bluetooth_state.setText("블루투스 ON");
                }else if(resultCode == RESULT_CANCELED){
                    Toast.makeText(getApplicationContext(), "취소", Toast.LENGTH_SHORT).show();
                    bluetooth_state.setText("블루투스 Off");
                }
                break;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }// onActivityResult End

    void listPairedDevices(){
        if(mBluetoothAdapter.isEnabled()){
            mPairedDevices = mBluetoothAdapter.getBondedDevices();

            if(mPairedDevices.size() > 0){
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setTitle("장치 선택");

                mListPairedDevices = new ArrayList<String>();
                for(BluetoothDevice device : mPairedDevices){
                    mListPairedDevices.add(device.getName());
                }
                final CharSequence[] items = mListPairedDevices.toArray(new CharSequence[mListPairedDevices.size()]);
                mListPairedDevices.toArray(new CharSequence[mListPairedDevices.size()]);

                builder.setItems(items, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int item) {
                        connectSelectedDevice(items[item].toString());
                    }
                });

                AlertDialog alert = builder.create();
                alert.show();
            }else {
                Toast.makeText(getApplicationContext(), "페어링된 장치가 없습니다.", Toast.LENGTH_SHORT).show();
            }
        }
        else{
            Toast.makeText(getApplicationContext(), "블루투스가 비활성화 되어 있습니다.", Toast.LENGTH_SHORT).show();
        }
    }// listPairedDevices End

    void connectSelectedDevice(String selectedDeviceName){
        for(BluetoothDevice tempDevice : mPairedDevices){
            if(selectedDeviceName.equals(tempDevice.getName())){
                mBluetoothDevice = tempDevice;
                connection_state.setText("연결되었습니다.");
                connNum = 1;
                Intent intent = new Intent(this,connect.class);
                intent.putExtra("data", connNum);
                break;
            }
        }
        try{
            mBluetoothSocket = mBluetoothDevice.createRfcommSocketToServiceRecord(BT_UUID);
            mBluetoothSocket.connect();
            mThreadConnectedBluetooth = new ConnectedBluetoothThread(mBluetoothSocket);
            mThreadConnectedBluetooth.start();
            mBluetoothHandler.obtainMessage(BT_CONNECTING_STATUS,1,-1).sendToTarget();
        }catch (IOException e){
            Toast.makeText(getApplicationContext(), "블루투스 연결 중 오류가 발생했습니다.", Toast.LENGTH_SHORT).show();
             connection_state.setText("연결에 실패했습니다.");
            connNum = 2;
            Intent intent = new Intent(this,connect.class);
            intent.putExtra("data", connNum);

        }
    }// connectSelectedDevice End

    public class ConnectedBluetoothThread extends Thread{
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedBluetoothThread(BluetoothSocket socket){
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            }catch (IOException e){
                Toast.makeText(getApplicationContext(), "소켓연결 중 오류가 발생했습니다.", Toast.LENGTH_SHORT).show();
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }//ConnectedBluetoothThread End

        public void run(){
            byte[] buffer = new byte[1024];
            int bytes;

            while(true){
                try {
                    bytes = mmInStream.available();
                    if(bytes != 0){
                        SystemClock.sleep(100);
                        bytes = mmInStream.available();
                        bytes = mmInStream.read(buffer,0,bytes);
                        mBluetoothHandler.obtainMessage(BT_MESSAGE_READ,bytes,-1,buffer).sendToTarget();
                    }
                }catch (IOException e){
                    break;
                }
            }
        }// run End

        public void write(String str){
            byte[] bytes = str.getBytes();
            try {
                mmOutStream.write(bytes);
            }catch (IOException e){
                Toast.makeText(getApplicationContext(), "데이터 전송 중 오류가 발생했습니다.", Toast.LENGTH_SHORT).show();
            }
        }// write End

        public void cancel(){
            try {
                mmSocket.close();
            }catch (IOException e){
                Toast.makeText(getApplicationContext(), "소켓 해제 중 오류가 발생했습니다.", Toast.LENGTH_SHORT).show();
            }
        }
    }//class ConnectedBluetoothThread End

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {

        switch (item.getItemId()){
            case android.R.id.home:{
                finish();
                return true;
            }
        }

        return super.onOptionsItemSelected(item);
    }

}
