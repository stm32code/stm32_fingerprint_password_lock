package com.example.wirelesscombinationlock.bean;

public class Send {
    private int cmd;
    private Integer set;
    private Integer door;

    @Override
    public String toString() {
        return "Send{" +
                "cmd=" + cmd +
                ", set=" + set +
                ", door=" + door +
                '}';
    }

    public int getCmd() {
        return cmd;
    }

    public void setCmd(int cmd) {
        this.cmd = cmd;
    }

    public Integer getSet() {
        return set;
    }

    public void setSet(Integer set) {
        this.set = set;
    }

    public Integer getDoor() {
        return door;
    }

    public void setDoor(Integer door) {
        this.door = door;
    }
}
