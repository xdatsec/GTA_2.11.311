package com.rockstargames.oswrapper.ui.tab;

import android.graphics.Color;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Filter;
import android.widget.Filterable;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;
import com.rockstargames.gtasa.R;

import java.util.ArrayList;
import java.util.List;

public class TabAdapter extends RecyclerView.Adapter implements Filterable {

    private Tab tab;
    private int selectedPosition = -1; // To track the clicked item's position

    private native void RpcClick(int id);
    private List<PlayerData> mPlayerData;
    private List<PlayerData> mPlayerDataCopy;

    public TabAdapter(List<PlayerData> playerData, Tab tab) {
        this.mPlayerData = playerData;
        this.mPlayerDataCopy = playerData;
        this.tab = tab;
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new ViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.tab_item, parent, false));
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        onBindViewHolder((ViewHolder) holder, position);
    }
    public static int sampToAndroidColor(int rgba) {
        int r = (rgba >> 24) & 0xFF;
        int g = (rgba >> 16) & 0xFF;
        int b = (rgba >> 8) & 0xFF;
        int a = rgba & 0xFF;
        return (a << 24) | (r << 16) | (g << 8) | b;
    }
    public void onBindViewHolder(ViewHolder holder, int position) {
        PlayerData data = this.mPlayerData.get(position);
        holder.id.setText(String.valueOf(data.getId()));
        holder.name.setText(data.getName());
        holder.level.setText(String.valueOf(data.getLevel()));
        holder.ping.setText(String.valueOf(data.getPing()));


        holder.id.setTextColor(Color.parseColor(data.getColor()));
        holder.name.setTextColor(Color.parseColor(data.getColor()));
        holder.level.setTextColor(Color.parseColor(data.getColor()));
        holder.ping.setTextColor(Color.parseColor(data.getColor()));

        if (position == selectedPosition) {
            holder.itemView.setBackgroundColor(Color.RED); // Highlight the selected item
        } else {
            holder.itemView.setBackgroundColor(Color.TRANSPARENT); // Default color
        }








    }


    @Override
    public int getItemCount() {
        return this.mPlayerData.size();
    }

    @Override
    public Filter getFilter() {
        return new Filter() {
            @Override
            public FilterResults performFiltering(CharSequence charSequence) {
                String input = charSequence.toString();
                List<PlayerData> newPlayersDataList = new ArrayList<>();
                if (input.isEmpty()) {
                    newPlayersDataList = mPlayerDataCopy;
                } else {
                    for (PlayerData playersData : mPlayerDataCopy) {
                        if (playersData.getName().toLowerCase().contains(input.toLowerCase())) {
                            newPlayersDataList.add(playersData);
                        }
                    }
                }
                FilterResults filterResults = new FilterResults();
                filterResults.values = newPlayersDataList;
                return filterResults;
            }

            @Override
            public void publishResults(CharSequence charSequence, FilterResults filterResults) {
                mPlayerData = (List) filterResults.values;
                notifyDataSetChanged();
            }
        };
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView id;
        public TextView level;
        public View mView;
        public ConstraintLayout View;
        public TextView name;
        public TextView ping;

        public ViewHolder(View itemView) {
            super(itemView);
            this.mView = itemView;
            this.View = (ConstraintLayout) itemView.findViewById(R.id.hassle_tab_item);
            this.id = (TextView) itemView.findViewById(R.id.player_id);
            this.name = (TextView) itemView.findViewById(R.id.player_name);
            this.level = (TextView) itemView.findViewById(R.id.player_level);
            this.ping = (TextView) itemView.findViewById(R.id.player_ping);
        }

        public View getView() {
            return this.mView;
        }
    }
}
