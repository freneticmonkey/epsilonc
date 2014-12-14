#pragma once

namespace epsilon
{
	template <typename T>
	class BaseManager
	{
	public:

		static T & GetInstance()
		{
			static T instance;
			return instance;
		};

		virtual void Setup(void) = 0;
		virtual void Update(float el) = 0;
		virtual void Destroy(void) = 0;

	protected:
		BaseManager();
		virtual ~BaseManager();

	private:
		BaseManager(BaseManager const&);
		void operator=(BaseManager const&);

	};
}