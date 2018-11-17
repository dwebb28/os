struct rpc_data{
	char content [100];
};

program COUNT_NUMBERS{
	version COUNT_VERS{
		int count(rpc_data)=1;
	}=1;
}=0x12344;