package com.example.bluetooth_with_raspberry_pi;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.content.Intent;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

public class connect extends AppCompatActivity {
    Toolbar tb;

    ImageButton btn_mode1;
    ImageButton btn_mode2;
    ImageButton object_out;
    ImageButton color_d;
    ImageButton cap_out;
    ImageButton sorting;
    ImageButton color_black;
    ImageButton color_red;
    ImageButton color_silver;

    ImageButton bluetooth_setting;

    TextView tv_conn_state;

    int sendNum = 0;
    int menuNum = 0;
    String sendString;

    Second.ConnectedBluetoothThread mThreadConnectedBluetooth;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_connect);

        btn_mode1 = findViewById(R.id.manual_m);
        btn_mode2 = findViewById(R.id.auto_m);
        object_out = findViewById(R.id.object_out);
        color_d = findViewById(R.id.color);
        cap_out = findViewById(R.id.cap_out);
        sorting = findViewById(R.id.sorting);
        color_black = findViewById(R.id.color_black);
        color_red = findViewById(R.id.color_red);
        color_silver = findViewById(R.id.color_silver);

        bluetooth_setting = findViewById(R.id.btn_bluetooth_st);

        tv_conn_state = findViewById(R.id.tv_connection_st);

        Intent intent = getIntent();
        final Intent intent2 = new Intent(this, Second.class);
        int num = intent.getIntExtra("data",0);

        if(num == 0 && num == 2){
            btn_mode1.setEnabled(false);
            btn_mode2.setEnabled(false);
            object_out.setEnabled(false);
            color_d.setEnabled(false);
            cap_out.setEnabled(false);
            sorting.setEnabled(false);
            color_black.setEnabled(false);
            color_red.setEnabled(false);
            color_silver.setEnabled(false);
            tv_conn_state.setText("연결되어 있지 않습니다.");
        }
        else if(num == 1){
            object_out.setEnabled(false);
            color_d.setEnabled(false);
            cap_out.setEnabled(false);
            sorting.setEnabled(false);
            color_black.setEnabled(false);
            color_red.setEnabled(false);
            color_silver.setEnabled(false);
            tv_conn_state.setText("연결되었습니다.");
        }

        bluetooth_setting.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(intent2);
            }
        });

        btn_mode1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 1;
                menuNum = 1;
                sendString = String.valueOf(sendNum);
                color_black.setEnabled(true);
                color_red.setEnabled(true);
                color_silver.setEnabled(true);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                Log.d(this.getClass().getName(),"Manual Mode");
            }
        });

        btn_mode2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 2;
                menuNum = 2;
                sendString = String.valueOf(sendNum);
                color_black.setEnabled(true);
                color_red.setEnabled(true);
                color_silver.setEnabled(true);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                Log.d(this.getClass().getName(),"Auto Mode");
            }
        });

        object_out.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 3;
                sendString = String.valueOf(sendNum);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                Log.d(this.getClass().getName(),"Object Out");
            }
        });

        color_d.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 4;
                sendString = String.valueOf(sendNum);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                Log.d(this.getClass().getName(),"Color Classification");
            }
        });

        cap_out.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 5;
                sendString = String.valueOf(sendNum);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                Log.d(this.getClass().getName(),"Cap Out");
            }
        });

        sorting.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 6;
                sendString = String.valueOf(sendNum);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                Log.d(this.getClass().getName(),"Sorting");
            }
        });

        color_black.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 7;
                sendString = String.valueOf(sendNum);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                if(menuNum == 1){
                    object_out.setEnabled(true);
                    color_d.setEnabled(true);
                    cap_out.setEnabled(true);
                    sorting.setEnabled(true);
                }
                else if(menuNum == 2){
                    object_out.setEnabled(false);
                    color_d.setEnabled(false);
                    cap_out.setEnabled(false);
                    sorting.setEnabled(false);
                }
                Log.d(this.getClass().getName(),"Color Black");
            }
        });

        color_red.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 8;
                sendString = String.valueOf(sendNum);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                if(menuNum == 1){
                    object_out.setEnabled(true);
                    color_d.setEnabled(true);
                    cap_out.setEnabled(true);
                    sorting.setEnabled(true);
                }
                else if(menuNum == 2){
                    object_out.setEnabled(false);
                    color_d.setEnabled(false);
                    cap_out.setEnabled(false);
                    sorting.setEnabled(false);
                }
                Log.d(this.getClass().getName(),"Color Red");
            }
        });

        color_silver.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendNum = 9;
                sendString = String.valueOf(sendNum);
                if(mThreadConnectedBluetooth != null){
                    mThreadConnectedBluetooth.write(sendString);
                }
                if(menuNum == 1){
                    object_out.setEnabled(true);
                    color_d.setEnabled(true);
                    cap_out.setEnabled(true);
                    sorting.setEnabled(true);
                }
                else if(menuNum == 2){
                    object_out.setEnabled(false);
                    color_d.setEnabled(false);
                    cap_out.setEnabled(false);
                    sorting.setEnabled(false);
                }
                Log.d(this.getClass().getName(),"Color Silver");
            }
        });

    }

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
