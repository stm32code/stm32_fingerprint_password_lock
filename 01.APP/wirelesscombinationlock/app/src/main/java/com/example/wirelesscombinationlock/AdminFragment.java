package com.example.wirelesscombinationlock;

import android.content.Intent;
import android.net.MailTo;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.example.wirelesscombinationlock.bean.Receive;
import com.example.wirelesscombinationlock.dao.UserDao;
import com.example.wirelesscombinationlock.databinding.FragmentAdminBinding;
import com.example.wirelesscombinationlock.utils.Common;
import com.example.wirelesscombinationlock.utils.MToast;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;


public class AdminFragment extends Fragment {
    private FragmentAdminBinding binding;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        binding = FragmentAdminBinding.inflate(inflater, container, false);
        initView();
        return binding.getRoot();
    }

    private void initView() {
        binding.doorBtn.setOnClickListener(view -> {
            binding.doorBtn.setSelected(!binding.doorBtn.isSelected());
            Common.sendMessage(2, binding.doorBtn.isSelected() ? "1" : "0");
            MToast.mToast(getContext(), binding.doorBtn.isSelected() ? "开门" : "关门");
        });

        binding.updateLockBtn.setOnClickListener(view -> {
            // 创建对话框构建器
            AlertDialog.Builder builder = new AlertDialog.Builder(getContext());
            // 获取布局
            View view2 = View.inflate(getContext(), R.layout.update_lock, null);
            // 获取布局中的控件
            final EditText lockText = (EditText) view2.findViewById(R.id.inputLock);
            final Button updateBtn = (Button) view2.findViewById(R.id.updateBtn);
            final Button cancelBtn = (Button) view2.findViewById(R.id.cancelBtn);
            lockText.setText(Common.Lock);
            // 设置参数
            builder.setTitle("修改通用密码").setView(view2);
            // 创建对话框
            final AlertDialog alertDialog = builder.create();
            cancelBtn.setOnClickListener(view1 -> alertDialog.dismiss());
            updateBtn.setOnClickListener(v -> {
                String psd = lockText.getText().toString().trim();
                if (psd.isEmpty()) {
                    MToast.mToast(getContext(), "不能为空");
                    alertDialog.dismiss();// 对话框消失
                    return;
                } else {
                    Common.user.setLock(psd);
                    new UserDao(getContext()).update(Common.user);
                    MToast.mToast(getContext(), "修改成功");
                    Common.sendMessage(1,psd);
                }
                alertDialog.dismiss();// 对话框消失
            });
            alertDialog.show();
        });
        binding.addUser.setOnClickListener(view -> {
            startActivity(new Intent(getActivity(), RegisterActivity.class));
        });
        binding.userManageBtn.setOnClickListener(view -> {
            startActivity(new Intent(getActivity(), UserManageActivity.class));
        });
    }

    /**
     * 解析数据
     *
     * @param result
     */
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void receiveDataFormat(Receive result) {
        try {
            if (result.getDoor() != null) {
                binding.doorBtn.setSelected(result.getDoor().equals("1"));
            }
        } catch (Exception e) {
            e.printStackTrace();
            MToast.mToast(getContext(), "数据解析失败");
        }
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EventBus.getDefault().register(this);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }
}