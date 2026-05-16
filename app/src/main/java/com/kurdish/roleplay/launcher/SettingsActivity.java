package com.kurdish.roleplay.launcher;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.widget.SwitchCompat;
import androidx.fragment.app.Fragment;

import com.joom.paranoid.Obfuscate;
import com.kurdish.roleplay.R;
import com.kurdish.roleplay.launcher.util.SharedPreferenceCore;

import org.ini4j.Wini;

import java.io.File;
import java.io.IOException;

@Obfuscate
public class SettingsActivity extends Fragment {

    Wini mWini = null;
    EditText mNickName;
    SwitchCompat mKeyboardSwitch;
    SwitchCompat mVoiceSwitch;
    SwitchCompat mFPSSwitch;
    SeekBar mMessagesSeekBar;
    TextView mMessagesText;
    SeekBar mFPSSeekBar;
    TextView mFPSText;
    View mReinstallCacheBtn;
    Messenger mService;
    Messenger mMessenger = new Messenger(new Handler());
    boolean mIsBound = false;




    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.activity_settings, container, false);


        initializeViews(view);
        loadSettings();
        setupListeners();

        if (getActivity() != null) {
            Intent intent = new Intent(getActivity(), UpdateService.class);
            getActivity().bindService(intent, mConnection, android.content.Context.BIND_AUTO_CREATE);
        }



        return view;
    }

    @Override
    public void onResume() {
        super.onResume();

        if (getActivity() != null) {
            mKeyboardSwitch.setChecked(new SharedPreferenceCore().getBoolean(getActivity(), "ANDROID_KEYBOARD"));
            mVoiceSwitch.setChecked(new SharedPreferenceCore().getBoolean(getActivity(), "VOICE"));
            mFPSSwitch.setChecked(new SharedPreferenceCore().getBoolean(getActivity(), "FPS_DISPLAY"));

            int fps = new SharedPreferenceCore().getInt(getActivity(), "FPS_LIMIT");
            switch (fps) {
                case 30: mFPSSeekBar.setProgress(0); break;
                case 60: mFPSSeekBar.setProgress(1); break;
                case 90: mFPSSeekBar.setProgress(2); break;
                case 120: mFPSSeekBar.setProgress(3); break;
            }
            mFPSText.setText(String.valueOf(fps));

            int message = new SharedPreferenceCore().getInt(getActivity(), "MESSAGE_COUNT");
            switch (message) {
                case 6: mMessagesSeekBar.setProgress(0); break;
                case 9: mMessagesSeekBar.setProgress(1); break;
                case 12: mMessagesSeekBar.setProgress(2); break;
                case 15: mMessagesSeekBar.setProgress(3); break;
            }
            mMessagesText.setText(String.valueOf(message));
        }
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requireActivity().getOnBackPressedDispatcher().addCallback(this,
                new androidx.activity.OnBackPressedCallback(true) {
                    @Override
                    public void handleOnBackPressed() {
                    }
                });
    }

    private void initializeViews(View view) {
        try {
            mNickName = view.findViewById(R.id.settings_nickname);
            mKeyboardSwitch = view.findViewById(R.id.keyboard_switch);
            mFPSSwitch = view.findViewById(R.id.fps_switch);
            mVoiceSwitch = view.findViewById(R.id.voice_switch);
            mMessagesSeekBar = view.findViewById(R.id.messages_seekbar);
            mMessagesText = view.findViewById(R.id.messages_count);
            mFPSSeekBar = view.findViewById(R.id.fps_seekbar);
            mFPSText = view.findViewById(R.id.fps_count);
            mReinstallCacheBtn = view.findViewById(R.id.btn_reinstall_cache);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private final android.content.ServiceConnection mConnection =
            new android.content.ServiceConnection() {
                @Override
                public void onServiceConnected(android.content.ComponentName name, IBinder service) {
                    mService = new Messenger(service);
                    mIsBound = true;
                }

                @Override
                public void onServiceDisconnected(android.content.ComponentName name) {
                    mService = null;
                    mIsBound = false;
                }
            };


    private void loadSettings() {
        try {
            if (getActivity() != null) {
                File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
                if (file.exists()) {
                    mWini = new Wini(file);
                    String name = mWini.get("client", "name");
                    if (name != null && mNickName != null) {
                        mNickName.setText(name);
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void setupListeners() {
        if (getActivity() != null) {
            MainActivity.hideKeyboard(getActivity());
        }

        mNickName.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                String text = charSequence.toString();
                if (getActivity() != null) {
                    File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
                    if(file.exists()) {
                        try {
                            if(mWini != null) {
                                mWini.put("client", "name", text);
                                mWini.store();
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }

            @Override
            public void afterTextChanged(Editable editable) {
            }
        });

        mKeyboardSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (getActivity() != null) {
                    new SharedPreferenceCore().setBoolean(getActivity(), "ANDROID_KEYBOARD", b);
                    try {
                        if(mWini != null) {
                            mWini.put("gui", "androidkeyboard", b);
                            mWini.store();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        mVoiceSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (getActivity() != null) {
                    new SharedPreferenceCore().setBoolean(getActivity(), "VOICE", b);
                    try {
                        if(mWini != null) {
                            mWini.put("gui", "VoiceChatEnable", b);
                            mWini.store();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        mFPSSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (getActivity() != null) {
                    new SharedPreferenceCore().setBoolean(getActivity(), "FPS_DISPLAY", b);
                    try {
                        if(mWini != null) {
                            mWini.put("gui", "fps", b ? 1 : 0);
                            mWini.store();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        mMessagesSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int realProgress = 0;
                switch(progress)
                {
                    case 0: realProgress = 6; break;
                    case 1: realProgress = 8; break;
                    case 2: realProgress = 10; break;
                    case 3: realProgress = 12; break;
                }

                if (getActivity() != null) {
                    new SharedPreferenceCore().setInt(getActivity(), "MESSAGE_COUNT", realProgress);

                    File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
                    if(file.exists()) {
                        try {
                            if(mWini != null) {
                                mWini.put("gui", "ChatMaxMessages", realProgress);
                                mWini.store();
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                    mMessagesText.setText(String.valueOf(realProgress));
                }
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        mReinstallCacheBtn.setOnClickListener(v -> {
            if (!mIsBound || mService == null || getActivity() == null) return;

            try {
                Message msg = Message.obtain(null, 8); // 🔥 REINSTALL CACHE
                msg.getData().putInt("gputype", 1); // ← ئەگەر GPU ـت لێرە نییە، 1 بنێرە (DXT)
                msg.replyTo = mMessenger;
                mService.send(msg);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        });


        mFPSSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int realProgress = 0;
                switch(progress)
                {
                    case 0: realProgress = 30; break;
                    case 1: realProgress = 60; break;
                    case 2: realProgress = 90; break;
                    case 3: realProgress = 120; break;
                }

                if (getActivity() != null) {
                    new SharedPreferenceCore().setInt(getActivity(), "FPS_LIMIT", realProgress);

                    File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
                    if(file.exists()) {
                        try {
                            if(mWini != null) {
                                mWini.put("gui", "FPSLimit", realProgress);
                                mWini.store();
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                    mFPSText.setText(String.valueOf(realProgress));
                }
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
    }
    @Override
    public void onDestroyView() {
        super.onDestroyView();
        if (mIsBound && getActivity() != null) {
            getActivity().unbindService(mConnection);
            mIsBound = false;
        }
    }

}