package com.example.wirelesscombinationlock;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;

import com.example.wirelesscombinationlock.dao.User;
import com.example.wirelesscombinationlock.dao.UserDao;
import com.example.wirelesscombinationlock.databinding.ActivityLoginBinding;
import com.example.wirelesscombinationlock.utils.Common;
import com.example.wirelesscombinationlock.utils.MToast;
import com.google.gson.Gson;
import com.gyf.immersionbar.ImmersionBar;

import java.util.List;

public class LoginActivity extends AppCompatActivity {
    private ActivityLoginBinding binding;
    private UserDao dao;
    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityLoginBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        sharedPreferences = getSharedPreferences("User", MODE_PRIVATE);
        editor = sharedPreferences.edit();
        String temp = sharedPreferences.getString("User", null);
        dao = new UserDao(this);
        Common.Lock = ((User) dao.query("admin", "name").get(0)).getLock();
        if (temp != null) {
            startActivity(new Intent(this, MainActivity.class));
            finish();
        }
        initView();
    }

    /****
     * 初始化视图
     */
    private void initView() {
        setSupportActionBar(binding.toolbar);
        binding.toolbarLayout.setTitle("登录");
        ImmersionBar.with(this).init();
        binding.loginBtn.setOnClickListener(view -> {
            verifyData();
        });
        /***
         * 跳转注册
         */
        binding.skipRegisterBtn.setOnClickListener(view -> {
            startActivity(new Intent(LoginActivity.this, RegisterActivity.class));
        });

    }

    private void verifyData() {
        String name = binding.inputNameEdit.getText().toString();
        String password = binding.inputPasswordEdit.getText().toString();
        if (name.isEmpty()) {
            MToast.mToast(this, "用户名不能为空");
            return;
        }
        if (password.isEmpty()) {
            MToast.mToast(this, "密码不能为空");
            return;
        }
        List<Object> objects = dao.query(name, "name");
        if (objects.size() == 0) {
            MToast.mToast(this, "没有该用户");
            return;
        }
        User user = (User) objects.get(0);
        if (user.getName().equals(name) && user.getPassword().equals(password)) {
            editor.putString("User", new Gson().toJson(user));
            editor.commit();
            startActivity(new Intent(this, MainActivity.class));
            finish();
        } else {
            MToast.mToast(this, "账号或密码错误");
        }
    }


}