package com.example.wirelesscombinationlock;

import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.text.InputType;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.PopupWindow;
import android.widget.TextView;

import com.example.wirelesscombinationlock.databinding.FragmentUserBinding;
import com.example.wirelesscombinationlock.utils.BeatingAnimation;
import com.example.wirelesscombinationlock.utils.Common;
import com.example.wirelesscombinationlock.utils.MToast;

import org.greenrobot.eventbus.EventBus;


public class UserFragment extends Fragment {
    private FragmentUserBinding binding;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        binding = FragmentUserBinding.inflate(inflater, container, false);
        initView();
        return binding.getRoot();
    }

    private void initView() {
        new BeatingAnimation().onAnimation(binding.openLockBtn);//让开锁键开始跳动
        binding.messageText.setText("密码:" + Common.user.getLock());
        binding.inputLockEdit.setOnEditorActionListener((textView, i, keyEvent) -> {
            if (i == EditorInfo.IME_ACTION_SEND) {
                String text = textView.getText().toString();
                if (!text.isEmpty()) {
                    if (Common.Lock != null && Common.Lock.equals(text)) {
                        Common.sendMessage(2, "1");
                        MToast.mToast(getContext(), "通用密码开门");
                    } else {
                        if (Common.user.getLock() != null && Common.user.getLock().equals(text)) {
                            if (Common.user.getOpen() == 1) {
                                Common.sendMessage(2, "1");
                                MToast.mToast(getContext(), "开门");
                            } else {
                                MToast.mToast(getContext(), "您没有权限，请联系管理员");
                            }
                        } else {
                            MToast.mToast(getContext(), "密码错误");
                        }
                        MToast.mToast(getContext(), "发送1");
                    }

                } else {
                    MToast.mToast(getContext(), "密码不能为空");
                }
                binding.inputLockEdit.clearFocus();//失去焦点
            }
            return false;
        });

        binding.openLockBtn.setOnClickListener(view -> {
            if (Common.user.getOpen() == 1) {
                Common.sendMessage(2, "1");
            } else {
                MToast.mToast(getContext(), "您没有权限，请联系管理员");
            }

        });
    }

}