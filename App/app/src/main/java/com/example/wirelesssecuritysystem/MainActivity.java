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
Button Lock;
Button btnReset;
DatabaseReference databaseReference;
FirebaseDatabase firebaseInstance;
TextView engineSt;
TextView MSG;
TextView SecureText;
String status;
String SecureStatus;
int val = 0;
long maxid = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Setting values to variables
        Lock = findViewById(R.id.SecureLock);
        btnReset = findViewById(R.id.RESET);
        firebaseInstance = FirebaseDatabase.getInstance();
        engineSt = (TextView) findViewById(R.id.EngineStatus);
        MSG = (TextView) findViewById(R.id.msg);
        SecureText = (TextView) findViewById(R.id.secureText);

        // Setting Firebase and its instance reference
        databaseReference= FirebaseDatabase.getInstance().getReference();
       // FirebaseDatabase.getInstance().setPersistenceEnabled(true);

        // Checking Engine Status by reading data from Firebase
        databaseReference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                status =dataSnapshot.child("Engine").child("status").getValue().toString();
                SecureStatus =dataSnapshot.child("SecureLock").child("status").getValue().toString();
                if(SecureStatus.equals("0"))
                {
                engineSt.setText(status);
                if(status.equals("1")){
                    notification(); // Push Notification
                    engineSt.setText("Disabled!");
                    MSG.setText("Press reset to initialise the system");
                }
                else{
                    engineSt.setText("Enable");
                    MSG.setText("");
                }
            }
                else{
                    engineSt.setText("Secured");
                    MSG.setText("Reset to Enable Engine");
                }
            }
            @Override   // Error Handling
            public void onCancelled(DatabaseError databaseError) {

            }
        });

        // Secure Lock
        Lock.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                firebaseInstance.getReference("SecureLock").child("status").setValue(1);
                SecureText.setText("Vehicle is now Fully Secured");
            }
        }));

        // System reset
        btnReset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                firebaseInstance.getReference("Engine").child("status").setValue(0);
                firebaseInstance.getReference("Reset").child("status").setValue(1);
                SecureText.setText("Secure Lock: OFF");
                firebaseInstance.getReference("SecureLock").child("status").setValue(0);
            }
        });
    }

    // Push Notification
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