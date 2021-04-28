#pragma once

namespace Salad {

	template<typename T>
	class Resource {
	
	public:
		T* getResource() {}

	private:
		T* resource;
	};

}