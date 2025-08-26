package com.example.wirelesscombinationlock.bean;

public class Receive {
    private String door; //锁状态
    private String pwd; //四位密码
    private String set;//通用密码

    public String getDoor() {
        return door;
    }

    public void setDoor(String door) {
        this.door = door;
    }

    public String getPwd() {
        return pwd;
    }

    public void setPwd(String pwd) {
        this.pwd = pwd;
    }

    public String getSet() {
        return set;
    }

    public void setSet(String set) {
        this.set = set;
    }
}
