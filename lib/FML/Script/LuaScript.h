#pragma once


#include "Script/LuaManager.h"
#include <sol2/sol.hpp>
#include <string>
#include <any>
#include <nlohmann/json_fwd.hpp>
#include <entt/fwd.hpp>

namespace fm
{
	class BaseEvent;
	class FilePath;
}

namespace fm
{

	class LuaScript : public Script
	{
	public:
		enum class ScriptTypeValue
		{
			NUMERAL,
			STRING,
			LUA,
			LAST
		};



		using Tokens = std::vector<std::string>;
		static void Tokens2Str(const Tokens& inTokens, std::string& outText);
		static Tokens CreateTokens(const std::string& inValueName);



		class ScriptArgument
		{
		public:
			Tokens location;
			ScriptTypeValue typeKind;
			std::any value;
		
		};



	    LuaScript(const fm::File &inFile, bool inParseInitValue);
	    ~LuaScript();
	    virtual void update(float dt) override;
	    virtual void start() override;
	    virtual bool init() override;
		virtual void Stop(const entt::handle& e) override;
		virtual bool Serialize(nlohmann::json &ioJson) const override;
		virtual bool Read(const nlohmann::json &inJSON) override;
		void CallEvent(entt::registry& registry, fm::BaseEvent* inEvent, sol::table &inTable);

	    fm::Script::SCRIPT_TYPE GetType() const override
	    {
	        return fm::Script::SCRIPT_TYPE::LUA;
	    }
		virtual bool Reload(bool inCreateInstance) override;
		void SetGoTable(sol::table &inTable);

		const std::map<std::string, LuaScript::ScriptArgument>& GetListValues() const;
		void EvaluateVariable(const ScriptArgument& inSpecialValue, std::any& out) const;
		bool SetValue(const ScriptArgument& inSpecialValue);
		
		bool Load(bool inParseInitValue);
		void SetStartValue(const std::string& inValueName, const std::any& inValue)
		{

			auto it = _valuesToInit.find(inValueName);
			if (it != _valuesToInit.end())
			{
				it->second.value = inValue;
			}
		}


	private:


		template <typename F, typename T, std::size_t N, std::size_t... Idx>
		decltype(auto) apply_impl(F f, T(&t)[N], std::index_sequence<Idx...>) {
			return f(t[Idx]...);
		}

		template <typename F, typename T, std::size_t N>
		decltype(auto) apply(F f, T(&t)[N]) {
			return apply_impl(f, t, std::make_index_sequence<N>{});
		}



		void _RegisterInitVariable(const sol::table& inTable, const std::string& inKey);
		template<typename T>
		std::optional<T> _EvaluateVariable(const LuaScript::Tokens& inTokens)
		{
			sol::table t = _table;
			std::optional<T> value;
			for (size_t i = 0; i < inTokens.size(); ++i)
			{
				if (!t.valid())
				{
					break;
				}

				if (i == inTokens.size() - 1)
				{
					value = t[inTokens[i]];
				}
				else
				{
					t = t[inTokens[i]];
				}
			}

			return value;
		}


	 private:
		bool									_hasAnErrorOccured;
		sol::table								_table;
		std::map<std::string, ScriptArgument>	_valuesToInit;
	};

	void to_json(nlohmann::json& j, const LuaScript::ScriptArgument& p);
	void from_json(const nlohmann::json& j, LuaScript::ScriptArgument& p);
}
