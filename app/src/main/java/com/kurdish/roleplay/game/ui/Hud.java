package com.kurdish.roleplay.game.ui;

import android.app.Activity;
import android.os.Handler;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.android.volley.Request;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.kurdish.roleplay.R;
import com.kurdish.roleplay.launcher.util.RemoteConfigManager;
import com.kurdish.roleplay.launcher.util.SeekArc;
import com.kurdish.roleplay.launcher.util.Util;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Formatter;

import org.json.JSONArray;
import org.json.JSONException;


public class Hud {

    public Activity activity;
    public ConstraintLayout hud_layout;
    public native void WeaponChanged();

    public ImageView hud_camera_butt;

    public TextView hud_money, hud_ammo, bank_money, time;
    public ImageView hud_weapon;
    public SeekArc hud_health, hud_armour, hud_eat, hud_drink;

    private final Handler handler = new Handler();

    public TextView textPlayers, PlayerId;

    private native void clickCameraMode();


    public Hud(Activity aactivity) {
        activity = aactivity;
        hud_layout = aactivity.findViewById(R.id.gamehud);

        if (hud_layout == null) {
            Log.e("Hud", "Hud layout is null! Check setContentView.");
            return;
        }

        hud_camera_butt = hud_layout.findViewById(R.id.hud_camera);
        textPlayers = hud_layout.findViewById(R.id.textPlayers);
        hud_health = hud_layout.findViewById(R.id.hud_progress_hp);
        hud_armour = hud_layout.findViewById(R.id.hud_progress_shp);
        hud_eat = hud_layout.findViewById(R.id.hud_progress_eat);
        hud_drink = hud_layout.findViewById(R.id.hud_progress_drink);
        hud_money = hud_layout.findViewById(R.id.money_text);
        bank_money = hud_layout.findViewById(R.id.bank_money_text);
        hud_weapon = hud_layout.findViewById(R.id.hud_weapon);
        PlayerId = hud_layout.findViewById(R.id.yourid);
        hud_ammo = hud_layout.findViewById(R.id.weapon_ammo_text);
        time = hud_layout.findViewById(R.id.realTime);
        hud_camera_butt.setOnClickListener(v -> clickCameraMode());

        startUpdatingTime();
        fetchOnline();

        Util.HideLayout(hud_layout, false);
    }

    private void startUpdatingTime() {
        handler.post(new Runnable() {
            @Override
            public void run() {
                updateTime();
                handler.postDelayed(this, 1000);
            }
        });
    }

    private void fetchOnline() {
        String url = RemoteConfigManager.getString("players");
        StringRequest request = new StringRequest(Request.Method.GET, url,
                response -> {
                    try {
                        int online = Integer.parseInt(response.trim());
                        textPlayers.setText(String.valueOf(online));
                    } catch (NumberFormatException e) {
                        try {
                            JSONArray array = new JSONArray(response);
                            int online = array.length();
                            textPlayers.setText(String.valueOf(online));
                        } catch (JSONException jsonException) {
                            Log.e("MainActivity", "JSON parse error: " + jsonException.getMessage());
                        }
                    }
                    handler.postDelayed(this::fetchOnline, 5000);
                },
                error -> {
                    Log.e("MainActivity", "Volley error: " + error.toString());
                    handler.postDelayed(this::fetchOnline, 5000);
                }
        );

        Volley.newRequestQueue(activity.getApplicationContext()).add(request);
    }

    private void updateTime() {
        if (time != null) {
            String currentTime = new SimpleDateFormat("HH:mm", Locale.getDefault())
                    .format(new Date());
            time.setText(currentTime);
        }
    }

    public void UpdateHudInfo(int health, int armour, int weaponid, int ammo, int ammoinclip,
                              int money, int eat, int drink, int bankMoney, int playerid)
    {
        hud_health.setProgress(health);
        hud_armour.setProgress(armour);
        hud_eat.setProgress(eat);
        hud_drink.setProgress(drink);

        DecimalFormat formatter = new DecimalFormat();
        DecimalFormatSymbols symbols = DecimalFormatSymbols.getInstance();
        symbols.setGroupingSeparator(' ');
        formatter.setDecimalFormatSymbols(symbols);

        String sMoney = formatter.format(money);
        hud_money.setText(String.format("%s $", sMoney));

        String sPlayerid = formatter.format(playerid);
        PlayerId.setText(String.format("%s", sPlayerid));

        String sBank = formatter.format(bankMoney);
        bank_money.setText(String.format("%s  $", sBank));

        int id = activity.getResources().getIdentifier(
                new Formatter().format("weapon_%d", Integer.valueOf(weaponid)).toString(),
                "drawable",
                activity.getPackageName());
        hud_weapon.setImageResource(id);
        hud_ammo.setText(String.format("%s / %s", ammo, ammoinclip));
        hud_weapon.setOnClickListener(v -> WeaponChanged());
    }

    public void ShowHud() {
        Util.ShowLayout(hud_layout, true);
    }

    public void HideHud() {
        Util.HideLayout(hud_layout, true);
    }
}

