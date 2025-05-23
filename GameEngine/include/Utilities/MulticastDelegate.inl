


template<typename ...ParamsTypes> inline MulticastDelegate<ParamsTypes...>::MulticastDelegate() : MulticastDelegate(false) { }

template<typename ...ParamsTypes> inline MulticastDelegate<ParamsTypes...>::MulticastDelegate(bool doDiscardUponTrigger_) {

	doDiscardUponTrigger = doDiscardUponTrigger_;
}

template <typename ...ParamsTypes> void MulticastDelegate<ParamsTypes...>::Add(std::function<void(ParamsTypes...)> function) {

	functions.push_back(function);
}

template<typename ...ParamsTypes> inline void MulticastDelegate<ParamsTypes...>::operator+=(std::function<void(ParamsTypes...)> function) {

	functions.push_back(function);
}

template <typename ...ParamsTypes> void MulticastDelegate<ParamsTypes...>::operator()(ParamsTypes ...args) {

	for (int i = 0; i < functions.size(); i++) {
		functions[i](args...);
	}

	if (doDiscardUponTrigger) {
		functions.clear();
	}
}

template<typename ...ParamsTypes> inline void MulticastDelegate<ParamsTypes...>::Invoke(ParamsTypes ...args) {

	(*this)(); // hehe
}
