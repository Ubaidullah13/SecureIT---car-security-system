package com.example.wirelesssecuritysystem;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import android.annotation.SuppressLint;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import org.w3c.dom.Text;


public class MainActivity extends AppCompatActivity {
Button btnON;
Button btnOFF;
Button btnReset;
DatabaseReference databaseReference;
FirebaseDatabase firebaseInstance;
TextView engineSt;
TextView MSG;
String status;
int val;
//https://www.androidhive.info/2016/10/android-working-with-firebase-realtime-database/
long maxid = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnOFF = findViewById(R.id.OFF);
        btnON = findViewById(R.id.ON);
        btnReset = findViewById(R.id.RESET);
        firebaseInstance = FirebaseDatabase.getInstance();
        engineSt = (TextView) findViewById(R.id.EngineStatus);
        MSG = (TextView) findViewById(R.id.msg);
        databaseReference= FirebaseDatabase.getInstance().getReference();
        FirebaseDatabase.getInstance().setPersistenceEnabled(true);
        databaseReference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                status =dataSnapshot.child("user").child("start").getValue().toString();
                engineSt.setText(status);
                if(status.equals("1")){
                    notification();
                    engineSt.setText("Disabled!");
                    MSG.setText("Press reset to initialise the system");
                }
                else{
                    engineSt.setText("Enable");
                    MSG.setText("");
                }
            }
            @Override
            public void onCancelled(DatabaseError databaseError) {

            }
        });

        btnOFF.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                firebaseInstance.getReference("user").child("start").setValue(0);
            }
        }));
        btnON.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                firebaseInstance.getReference("user").child("start").setValue(1);
            }
        }));

        btnReset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                firebaseInstance.getReference("Door").child("int").setValue(0);
                firebaseInstance.getReference("EngineStarts").child("int").setValue(0);
                firebaseInstance.getReference("Lock").child("int").setValue(0);
                firebaseInstance.getReference("test").child("int").setValue(0);
                firebaseInstance.getReference("user").child("start").setValue(0);
            }
        });
    }

    private void notification(){
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            Intent intent = new Intent(this, MainActivity.class);
            PendingIntent Pintent = PendingIntent.getActivity(this,1,intent,PendingIntent.FLAG_CANCEL_CURRENT);
            NotificationChannel channel = new NotificationChannel("Engine Status", "Engine Status", NotificationManager.IMPORTANCE_HIGH);
            NotificationManager manager = getSystemService(NotificationManager.class);
            manager.createNotificationChannel(channel);
            NotificationCompat.Builder builder = new NotificationCompat.Builder(this, "Engine Status").setContentTitle("Alert! Security Breach").setSmallIcon(R.drawable.alert)
                    .setAutoCancel(true)
                    .setContentText("Someone is tried to steal your car. Engine is disabled, you can reset the system using app but make sure check the car first")
                    .setContentIntent(Pintent);
            NotificationManagerCompat managerCompat = NotificationManagerCompat.from(this);
            managerCompat.notify(999,builder.build());
        }
    }
}