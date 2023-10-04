#pragma once
#include <memory>
namespace fm
{
	class Model;
	class OGLModel
	{
	public:
		OGLModel(std::shared_ptr<fm::Model> inModel);
		bool	UploadData();
		bool	Destroy();
		void	Draw();


	private:
		void	_BindIndex(size_t index) const;
		void	_PrepareBuffer();
		std::shared_ptr<Model> _model = nullptr;
	};
}