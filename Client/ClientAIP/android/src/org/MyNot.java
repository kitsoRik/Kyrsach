package org;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.RemoteInput;
import android.widget.RemoteViews;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.media.RingtoneManager;
import org.qtproject.example.mynot.R;
import android.net.Uri;

public class MyNot extends org.qtproject.qt5.android.bindings.QtActivity {


    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_builder;
    public static MyNot m_instance;

    public MyNot() {
        m_instance = this;
    }

public static void notify1()
{

    if(m_notificationManager == null)
    {
        Uri alarmSound = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
        m_builder = new Notification.Builder(m_instance);
        m_builder.setSmallIcon(R.drawable.icon);
        m_builder.setContentTitle("Trigggered");
        m_builder.setSound(alarmSound);
        }

    m_builder.setContentText("TITLE");
    m_notificationManager.notify(1, m_builder.build());
    }
}
